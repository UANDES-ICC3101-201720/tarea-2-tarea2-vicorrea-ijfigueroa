import os
import time


algoritmos = ["fifo", "random", "custom"]
programas = ["scan", "focus", "sort" ]
marcos = []
dato=[]
for algoritmo in algoritmos:
	for programa in programas:
		ejecutar = "./virtmem 100 {} {} {} >> {}_{}.txt "
		for i in range(2,101):
			eje = ejecutar.format(i, algoritmo, programa, algoritmo, programa)
			os.system(eje)
			print eje

		


