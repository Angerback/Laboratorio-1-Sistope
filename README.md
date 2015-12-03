# README #


## Laboratorio 1 Sistope - Procesos ##

* Este programa implementa una calculadora de matrices. Incluye funciones de suma, resta, multiplicación y transpuesta. Contiene una memoria de 26 matrices de la A a la Z (Ñ no incluye) y extrae los datos desde archivos de texto.
* Version 1.0
* [Learn Markdown](https://bitbucket.org/tutorials/markdowndemo)

## ¿Cómo iniciar? ##

* Compilar en la raíz del proyecto con el comando make. Luego ejecutar el programa "padre" del directorio ./build.

### Comandos ###

El programa recibe comandos al estilo de una shell, por lo que para operar se deben seguir los siguientes comandos.

* load <MATRIZ> <ARCHIVO>
El comando load carga un archivo de texto, lo decodifica y guarda la matriz contenida en un slot de memoria. MATRIZ = [A-Z] y ARCHIVO debe ser el nombre del archivo en cuestión. Se recomienda que el archivo esté en la carpeta build.
Ejemplo: load A archivo.txt

* save <MATRIZ> <ARCHIVO>
El comando save guarda una matriz existente en memoria en un archivo de texto. Si el archivo no existe, lo creará. MATRIZ = [A-Z] y ARCHIVO es el nombre del archivo a crear/actualizar. La matriz debe existir en memoria.
Ejemplo: load B bguardado.txt

* 