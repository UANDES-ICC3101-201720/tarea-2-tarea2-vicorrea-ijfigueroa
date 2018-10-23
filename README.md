Integrantes:
	-Vicente Correa
	-Ignacio Figueroa

Hipótesis: Nuestro algorimo custom consiste en un contador que va recorriendo un rango de 0 a
99 secuencialmente. La idea es que este contador elige el número de marco que será utilizado
en la siguiente falta de página. Para el algoritmo custom se espera que este se comporte parecido
al algoritmo FIFO, del cual esperamos que inicialmente el número de lecturas a disco
sea parecido al número de escrituras a disco/faltas de página, y a medida que vaya aumentando
la cantidad de marcos, la cantidad de escrituras a disco vaya siendo menor a los otros
dos parámetros, ya que funcionan más o menos de la misma manera. Para el algoritmo random, se
espera que a medida que aumente la cantidad de marcos, la cantidad de faltas de página y
lecturas a disco disminuyan.

Resultados: Los 3 algoritmos se comportaron de manera similar. Como esperabamos, el algoritmo
FIFO fue igual al custom debido a como estos dos funcionan. El gráfico del algoritmo random es
más interesante de analizar ya que en este podemos ver que para algunos números de marcos se 
produce la anomalía de Belamy, la cual plantea que a veces, una mayor cantidad de marcos no 
significa necesariamente una menor cantidad de faltas de páginas. Esta anomalía se acentúa
a medida que la cantidad de marcos es mayor y la cantidad de faltas es mayor. 
Una cosa importante a destacar es que la cantidad de faltas de página es, para todos los 
gráficos, siempre igual a la cantidad de lecturas a disco. Esto es debido a cómo funcionan
los programas base que nos fueron entregados; como no existe manera de saber que operación
está realizando un programa (escritura, lectura o ejecución) tuvimos que darle todos los permisos
cada vez que leíamos del disco, por ende, como siempre se leíamos del disco y nunca de la memoria,
cada vez que se gatillaba el page_fault_handler, el único caso posible era que la página no estuviera 
en memoria. 
También es importante mencionar que las escrituras a disco son menores que las 
faltas de página/lecturas de disco, ya que en un principio existen marcos vacíos, por lo que no es 
necesario escribir en disco. 
Podemos ver que si aproximamos una interpolación de los 3 algoritmos son muy parecidos.

Gráficos:


<img src="https://github.com/UANDES-ICC3101-201720/tarea-2-tarea2-vicorrea-ijfigueroa/blob/master/Graficos/focus%20usando%20fifo.png" />

<img src="https://github.com/UANDES-ICC3101-201720/tarea-2-tarea2-vicorrea-ijfigueroa/blob/master/Graficos/focus%20usando%20random.png" />

<img src="https://github.com/UANDES-ICC3101-201720/tarea-2-tarea2-vicorrea-ijfigueroa/blob/master/Graficos/focus%20usando%20custom.png" />

<img src="https://github.com/UANDES-ICC3101-201720/tarea-2-tarea2-vicorrea-ijfigueroa/blob/master/Graficos/sort%20usando%20fifo.png" />

<img src="https://github.com/UANDES-ICC3101-201720/tarea-2-tarea2-vicorrea-ijfigueroa/blob/master/Graficos/sort%20usando%20random.png" />

<img src="https://github.com/UANDES-ICC3101-201720/tarea-2-tarea2-vicorrea-ijfigueroa/blob/master/Graficos/sort%20usando%20custom.png" />

<img src="https://github.com/UANDES-ICC3101-201720/tarea-2-tarea2-vicorrea-ijfigueroa/blob/master/Graficos/scan%20usando%20fifo.png" />

<img src="https://github.com/UANDES-ICC3101-201720/tarea-2-tarea2-vicorrea-ijfigueroa/blob/master/Graficos/scan%20usando%20random.png" />

<img src="https://github.com/UANDES-ICC3101-201720/tarea-2-tarea2-vicorrea-ijfigueroa/blob/master/Graficos/scan%20usando%20custom.png" />
