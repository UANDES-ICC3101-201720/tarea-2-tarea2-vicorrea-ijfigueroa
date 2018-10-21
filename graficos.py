from matplotlib import pyplot
#import matplotlib.image as mpimg

algoritmos = ["fifo", "random", "custom"]
programas = ["scan", "focus", "sort"]



for a in algoritmos:
	for p in programas:
		faltas_paginas = []
		lecturas_disco = []
		leidas_discos = []
		ruta = a + "_" + p + ".txt"
		titulo = p + " usando " + a
		archivo = open(ruta) 
		for linea in archivo:
			linea = linea.strip("\n")
			if p not in linea:
				l = linea.split(" ")
				faltas_paginas.append(int(l[0]))
				lecturas_disco.append(int(l[1]))
				leidas_discos.append(int(l[2]))
		"""		
		print "Faltas de paginas:\n"
		print faltas_paginas
		print "Lecturas disco:\n"
		print lecturas_disco
		print "Leidas disco:\n"
		print leidas_discos
		"""
		pyplot.title(titulo)
		pyplot.xlabel("Cantidad de marcos de memoria")
		pyplot.ylabel("Cantidad")
		pyplot.plot(range(2,101), faltas_paginas, "r", label = "# faltas de pagina")
		pyplot.plot(range(2,101), lecturas_disco, "g", label = "# lecturas de disco")
		pyplot.plot(range(2,101), leidas_discos, "b", label = "# leidas de disco")
		pyplot.plot()
		pyplot.show()



#fig = pyplot.gcf()
#fig.savefig("hola.jpg")
