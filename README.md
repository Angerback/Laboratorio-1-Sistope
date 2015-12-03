# README #


## Laboratorio 1 Sistope - Procesos ##

* Este programa implementa una calculadora de matrices. Incluye funciones de suma, resta, multiplicación y transpuesta. Contiene una memoria de 26 matrices de la A a la Z (Ñ no incluye) y extrae los datos desde archivos de texto.
* Version 1.0

## ¿Cómo iniciar? ##

* Compilar en la raíz del proyecto con el comando make. Luego ejecutar el programa "padre" del directorio ./build.

### Comandos ###

El programa recibe comandos al estilo de una shell, por lo que para operar se debe seguir la siguiente estructura.

* load <MATRIZ> <ARCHIVO>
El comando load carga un archivo de texto, lo decodifica y guarda la matriz contenida en un slot de memoria. MATRIZ = [A-Z] y ARCHIVO debe ser el nombre del archivo en cuestión. Se recomienda que el archivo esté en la carpeta build.
Ejemplo: load A archivo.txt

* save <MATRIZ> <ARCHIVO>
El comando save guarda una matriz existente en memoria en un archivo de texto. Si el archivo no existe, lo creará. MATRIZ = [A-Z] y ARCHIVO es el nombre del archivo a crear/actualizar. La matriz debe existir en memoria.
Ejemplo: load B bguardado.txt

* print <MATRIZ>
El comando print muestra el contenido de la matriz por pantalla. Si la matriz no ha sido inicializada, mostrará un error.
Ejemplo: print J

* clear <A>
El comando clear limpia una matriz guardada en memoria, es decir, la vacía.
Ejemplo: clear D

* <MATRIZA> = <MATRIZB>
El operador = guarda el contenido de <MATRIZB> en <MATRIZA>. Se realiza una copia, y la matriz presente en <MATRIZA> es independiente de su contraparte original en <MATRIZB>.
Ejemplo: T=C

* <MATRIZA>=<MATRIZB>+<MATRIZC>
El operador suma, realiza una adición de los valores de cada casilla en las matrices <MATRIZB> y <MATRIZC>, creando una nueva matriz con el resultado y guardándola en <MATRIZA>.

* <MATRIZA>=<MATRIZB>-<MATRIZC>
El operador resta, realiza una sustracción de los valores de cada casilla en las matrices <MATRIZB> y <MATRIZC>, creando una nueva matriz con el resultado y guardándola en <MATRIZA>.

* <MATRIZA>=<MATRIZB>*<MATRIZC>
El operador multiplicación, realiza una multiplicación de las matrices <MATRIZB> y <MATRIZC>, creando una nueva matriz con el resultado y guardándola en <MATRIZA>.

* <MATRIZA>=<MATRIZB>*<CONSTANTE>
El operador multiplicación, cuando está emparejado con una constante a la derecha, realiza una multiplicación de los valores de cada casilla en la matriz <MATRIZB> y la constante, creando una nueva matriz con el resultado y guardándola en <MATRIZA>.

* <MATRIZA>=trans <MATRIZB>
El operador trans, calcula la matriz transpuesta de la matriz <MATRIZB>, creando una nueva matriz con el resultado y guardándola en <MATRIZA>.