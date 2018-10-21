/*
Main program for the virtual memory project.
Make all of your modifications to this file.
You may add or rearrange any code or data as you need.
The header files page_table.h and disk.h explain
how to use the page table and disk interfaces.
*/

#include "page_table.h"
#include "disk.h"
#include "program.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <limits.h>

struct disk * disk;
int * frame_table;
int nframes;
char *physmem;

int *queue;
int cabeza = -1;
int cola = -1;

int enQueue(int valor){
	if (cola - cabeza == nframes - 1){ // esta lleno
		//printf("No se puede insertar el %d, el queue esta lleno\n", valor);
		return -1;
	}
	else{
		if (cabeza == -1){
			cabeza = 0;
		}
		cola++;
		queue[cola] = valor;
		//printf("Agregando queue[%d] = %d\n", cola, valor);
		return valor;
	}
}
int deQueue(){
	int ret = -1;
	if (cabeza == -1){ // esta vacio
		//printf("No se puede elimiar, el queue esta vacio\n");
	}
	else{
		ret = queue[cabeza];
		//printf("Eliminando queue[%d] = %d\n", cabeza, ret);
		cabeza++;
		if (cabeza > cola){
			cabeza = cola = -1;
		}
	}
	return ret;
}
void impirmirQueue(){
	if (cola == -1){
		//printf("El queue esta vacio\n");
	}
	else{
		//printf("El queue contiene lo siguiente:\n");
		for (int i = cabeza; i <= cola; i++){
			printf("%d ", queue[i]);
		}
		printf("\n");
	}
}

/*
	Cuando se trata de leer una pagina, esta funcion le asigna 
	un marco a aquella pagina si esta no tiene un marco y luego
	cargara la pagina desde el disco al marco asignado 
	*/
int marco = 0;
int cont_marco_vic = 0;

int page_fault_count = 0;
int read_disk_count = 0;
int write_disk_count = 0;

void page_fault_handler_FIFO(struct page_table *pt, int page)
{
	page_fault_count++;
	//printf("page fault on page #%d\n",page);
	//printf("USANDO PAGE FAULT HANDLER FIFO:\n");
	if (enQueue(marco) != -1){ // si se pudo meter al queue
		page_table_set_entry(pt, page, marco, PROT_READ|PROT_WRITE|PROT_EXEC);
		disk_read(disk, page, &physmem[marco*PAGE_SIZE]);
		read_disk_count++;
		frame_table[marco] = page;
		marco++;
	}
	else{
		int marco_victima = deQueue();
		disk_write(disk, frame_table[marco_victima], &physmem[marco_victima*PAGE_SIZE]);
		write_disk_count++;
		disk_read(disk, page, &physmem[marco_victima*PAGE_SIZE]);
		read_disk_count++;
		page_table_set_entry(pt, page, marco_victima, PROT_READ|PROT_WRITE|PROT_EXEC);
		page_table_set_entry(pt, frame_table[marco_victima], marco_victima, 0);
		frame_table[marco_victima] = page;
		enQueue(marco_victima);
	}
}

void page_fault_handler_CUSTOM(struct page_table *pt, int page){
	page_fault_count++;
	//printf("page fault on page #%d\n",page);
	//printf("USANDO PAGE FAULT HANDLER CUSTOM:\n");
	if (marco == nframes){ 
		int marco_victima = cont_marco_vic;
		disk_write(disk, frame_table[marco_victima], &physmem[marco_victima*PAGE_SIZE]);
		write_disk_count++;
		disk_read(disk, page, &physmem[marco_victima*PAGE_SIZE]);
		read_disk_count++;
		page_table_set_entry(pt, page, marco_victima, PROT_READ|PROT_WRITE|PROT_EXEC);
		page_table_set_entry(pt, frame_table[marco_victima], marco_victima, 0);
		frame_table[marco_victima] = page;
		cont_marco_vic++;
		if (cont_marco_vic == nframes){
			cont_marco_vic = 0;
		}
	}
	else{
		page_table_set_entry(pt, page, marco, PROT_READ|PROT_WRITE|PROT_EXEC);
		disk_read(disk, page, &physmem[marco*PAGE_SIZE]);
		read_disk_count++;
		frame_table[marco] = page;
		marco++;
	}
}

void page_fault_handler_RANDOM(struct page_table *pt, int page){
	page_fault_count++;
	//printf("page fault on page #%d\n",page);
	//printf("USANDO PAGE FAULT HANDLER RANDOM:\n");
	if (marco == nframes){
		int marco_victima = lrand48()%nframes;
		disk_write(disk, frame_table[marco_victima], &physmem[marco_victima*PAGE_SIZE]);
		write_disk_count++;
		disk_read(disk, page, &physmem[marco_victima*PAGE_SIZE]);
		read_disk_count++;
		page_table_set_entry(pt, page, marco_victima, PROT_READ|PROT_WRITE|PROT_EXEC);
		page_table_set_entry(pt, frame_table[marco_victima], marco_victima, 0);
		frame_table[marco_victima] = page;
	}
	else{
		page_table_set_entry(pt, page, marco, PROT_READ|PROT_WRITE|PROT_EXEC);
		disk_read(disk, page, &physmem[marco*PAGE_SIZE]);
		read_disk_count++;
		frame_table[marco] = page;
		marco++;
	}
	
}

int main( int argc, char *argv[] )
{

	if(argc!=5) {
		/* Add 'random' replacement algorithm if the size of your group is 3 */
		printf("use: virtmem <npages> <nframes> <random|custom|fifo> <sort|scan|focus>\n");
		return 1;
	}

	int npages = atoi(argv[1]);
	nframes = atoi(argv[2]);
	const char *algorithm = argv[3];
	const char *program = argv[4];

	frame_table = malloc(sizeof(int) * nframes);

	disk = disk_open("myvirtualdisk",npages);
	if(!disk) {
		fprintf(stderr,"couldn't create virtual disk: %s\n",strerror(errno));
		return 1;
	}

	struct page_table *pt;
	if (!strcmp(algorithm, "fifo")){
		pt = page_table_create( npages, nframes, page_fault_handler_FIFO );
	}
	else if (!strcmp(algorithm, "custom")){
		pt = page_table_create( npages, nframes, page_fault_handler_CUSTOM );
	}
	else if (!strcmp(algorithm, "random")){
		pt = page_table_create( npages, nframes, page_fault_handler_RANDOM );
	}
	else {
		fprintf(stderr, "unknown algorithm: %s\n",algorithm);
		return 1;
	}

	if(!pt) {
		fprintf(stderr,"couldn't create page table: %s\n",strerror(errno));
		return 1;
	}

	//Inicio de la memoria virtual de la tabla de paginas pt
	char *virtmem = page_table_get_virtmem(pt); 
	//Inicio de la memoria fisica  de la tabla de paginas pt
	physmem = page_table_get_physmem(pt);

	queue = malloc(sizeof(int)*10000);

	if (!strcmp(program,"sort")) {
		sort_program(virtmem,npages*PAGE_SIZE);

	} else if(!strcmp(program,"scan")) {
		scan_program(virtmem,npages*PAGE_SIZE);

	} else if(!strcmp(program,"focus")) {
		focus_program(virtmem,npages*PAGE_SIZE);

	} else {
		fprintf(stderr,"unknown program: %s\n", program);

	}

	/*
	printf("----------- PAGE TABLE ----------\n");
	page_table_print(pt);
	printf("\n");

	printf("----------- FRAME TABLE ---------\n");
	for (int t = 0; t < nframes; t++){
		printf("frame_table[%d] = %d\n", t, frame_table[t]);
	}
	*/
	
	printf("%d %d %d %d\n", page_fault_count, read_disk_count, write_disk_count, nframes);

	free(frame_table);
	free(queue);

	page_table_delete(pt);
	disk_close(disk);

	return 0;
}
