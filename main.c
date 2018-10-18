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

struct disk * disk;
int * frame_table;
int nframes;
//#define q_size 5
int *queue;
int cabeza = -1;
int cola = -1;

void enQueue(int valor){
	if (cola - cabeza == nframes - 1){ // esta lleno
		printf("No se puede insertar el %d, el queue esta lleno\n", valor);
	}
	else{
		if (cabeza == -1){
			cabeza = 0;
		}
		cola++;
		queue[cola] = valor;
		printf("Agregando queue[%d] = %d\n", cola, valor);
	}
}
int deQueue(){
	int ret = -1;
	if (cabeza == -1){ // esta vacio
		printf("No se puede elimiar, el queue esta vacio\n");
	}
	else{
		ret = queue[cabeza];
		printf("Eliminando queue[%d] = %d\n", cabeza, ret);
		cabeza++;
		if (cabeza > cola){
			cabeza = cola = -1;
		}
	}
	return ret;
}
void impirmirQueue(){
	if (cola == -1){
		printf("El queue esta vacio\n");
	}
	else{
		printf("El queue contiene lo siguiente:\n");
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
void page_fault_handler_FIFO(struct page_table *pt, int page)
{
	/*
	page_table_print(pt);
	char ** frame_table = page_table_get_frame_table(pt);
	int nframes = page_table_get_nframes(pt);
	char *physmem = page_table_get_physmem(pt);
	int ftFull = 0;
	if (!ftFull){
		for (int i = 0; i<nframes;i++)
			if(frame_table[i]==NULL){
				page_table_set_entry(pt, page, i, PROT_READ);
				disk_read(disk,page,&physmem[i]);
			}
	}
	*/

	printf("page fault on page #%d\n",page);
	exit(1);

}

void page_fault_handler_CUSTOM(struct page_table *pt, int page){

}

void page_fault_handler_RANDOM(struct page_table *pt, int page){

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

	int frame_table[nframes];

	struct disk *disk = disk_open("myvirtualdisk",npages);
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
	char *physmem = page_table_get_physmem(pt);
	//char *frame_table = malloc(nframes*sizeof(char));

	queue = malloc(sizeof(int)*nframes);

	deQueue();
	enQueue(4);
	enQueue(13);
	deQueue();
	enQueue(7);
	impirmirQueue();
	deQueue();
	enQueue(90);
	enQueue(100);
	impirmirQueue();
	enQueue(8);
	enQueue(1);
	enQueue(77);
	impirmirQueue();
	for (int a = 0; a < nframes; a++) {
		deQueue();
	}
	impirmirQueue();
	
	
	for (int i = 0; i < nframes; i++){
		page_table_set_entry(pt, i, i, PROT_WRITE | PROT_READ);
		
	}
	printf("---------------------\n");
	page_table_print(pt);
	printf("---------------------\n");

	for (int i = 0; i < npages; i++){
		page_table_print_entry(pt, i);
		int f;
		int b;
		page_table_get_entry(pt, i, &f, &b);
		printf("frame numero: %d, permisos de acceso: %d\n", f, b);
		if (b != 0){
			frame_table[f] = f;
		}
	}
	
	for (int t = 0; t < nframes; t++){
		printf("frame_table[%d] = %d\n", t, frame_table[t]);
	}

	if (!strcmp(program,"sort")) {
		sort_program(virtmem,npages*PAGE_SIZE);

	} else if(!strcmp(program,"scan")) {
		scan_program(virtmem,npages*PAGE_SIZE);

	} else if(!strcmp(program,"focus")) {
		focus_program(virtmem,npages*PAGE_SIZE);

	} else {
		fprintf(stderr,"unknown program: %s\n", program);

	}

	page_table_delete(pt);
	disk_close(disk);

	return 0;
}
