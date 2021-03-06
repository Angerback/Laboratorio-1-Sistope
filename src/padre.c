#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <errno.h>

#include "matriz.h"

#define READ 0
#define WRITE 1

#define DUP2CLOSE(oldfd, newfd)	( dup2(oldfd, newfd), close(oldfd) )

// La memoria de matrices del programa es estática
matriz * matrices;

char getIndex(char matriz){
  if(matriz < 65 || matriz > 90){
    return -1;
  }else{
    return matriz - 65; // 65 es el valor ASCII de la letra A
  }
}

int iniciar_matriz(char matriz, int n, int m){

  int i;
  matrices[matriz].n = n;
  matrices[matriz].m = m;

  if(matrices[matriz].matriz != NULL)
    free(matrices[matriz].matriz);
  matrices[matriz].matriz = (int **)malloc(n * sizeof(int*));
  for(i = 0; i < n; i++){
    matrices[matriz].matriz[i] = (int *)malloc(m * sizeof(int));
  }

}

int operar(char * string, char * programa){

  //Para hacer suma, le paso las matrices por pipes y que sume.
  int piped[2]; // Para escribir en el hijo
  int piped2[2]; // Para leer desde el hijo
  int p1 = pipe(piped);
  int p2 = pipe(piped2);
  if(p1 == -1 || p2 == -1){
    printf("ERROR EN PIPES\n");
  }
  pid_t pid = fork();
  if(pid == 0){
    close(piped2[READ]);
    close(piped[WRITE]);
    // El hijo ejecuta el programa:
    //dup2(piped[READ],STDIN_FILENO);
    DUP2CLOSE(piped[READ],STDIN_FILENO);
    DUP2CLOSE(piped2[WRITE],STDOUT_FILENO);
    //printf("%d %d\n", STDIN_FILENO, STDOUT_FILENO);

    //while ((dup2(piped[READ],STDIN_FILENO) == -1) && (errno == EINTR)) {printf("HOLI\n");}

    //while ((dup2(piped2[WRITE],STDOUT_FILENO) == -1) && (errno == EINTR)) {printf("HOLI\n");}

    //El hijo lee lo que le llega.
    execl(programa, programa, (char *) 0);
  }else{
    close(piped2[WRITE]);
    close(piped[READ]);

    //close(piped2[WRITE]);
    //int a = 2;
    //write(piped[WRITE], &a, sizeof(int));
    char a = getIndex(string[0]), b = getIndex(string[2]), c = getIndex(string[4]);
    int n = matrices[b].n;
    int m = matrices[b].m;
    int i , j;
    //Se escriben n y m
    write(piped[WRITE], &n, sizeof(int));
    write(piped[WRITE], &m, sizeof(int));
    // Luego se escriben todos los valores de la matriz de forma secuencial
    for (i = 0; i < n; i++) {
      for (j = 0; j < m; j++) {
        int aux = matrices[b].matriz[i][j];
        write(piped[WRITE], &aux, sizeof(int));
      }
    }

    //Se pasa la otra matriz:
    n = matrices[c].n;
    m = matrices[c].m;

    write(piped[WRITE], &n, sizeof(int));
    write(piped[WRITE], &m, sizeof(int));
    // Luego se escriben todos los valores de la matriz de forma secuencial
    for (i = 0; i < n; i++) {
      for (j = 0; j < m; j++) {
        int aux = matrices[c].matriz[i][j];
        write(piped[WRITE], &aux, sizeof(int));
      }
    }
    // Finalmente, guarda el valor calculado por el hijo en la variable local.
    int nres, mres;
    read(piped2[READ], &nres, sizeof(int));
    read(piped2[READ], &mres, sizeof(int));
    iniciar_matriz(a, nres, mres);

    for (i = 0; i < nres; i++) {
      for (j = 0; j < mres; j++) {
        int aux;
        read(piped2[READ], &aux, sizeof(int));
        matrices[a].matriz[i][j] = aux;
        //printf("%d\n", aux);
      }
    }

  }

}


int operar2(char * string, char * programa, int constante){
  //Para hacer suma, le paso las matrices por pipes y que sume.
  int piped[2]; // Para escribir en el hijo
  int piped2[2]; // Para leer desde el hijo
  int p1 = pipe(piped);
  int p2 = pipe(piped2);
  if(p1 == -1 || p2 == -1){
    printf("ERROR EN PIPES\n");
  }
  pid_t pid = fork();
  if(pid == 0){
    close(piped2[READ]);
    close(piped[WRITE]);
    // El hijo ejecuta el programa:
    //dup2(piped[READ],STDIN_FILENO);
    DUP2CLOSE(piped[READ],STDIN_FILENO);
    DUP2CLOSE(piped2[WRITE],STDOUT_FILENO);
    //printf("%d %d\n", STDIN_FILENO, STDOUT_FILENO);

    //while ((dup2(piped[READ],STDIN_FILENO) == -1) && (errno == EINTR)) {printf("HOLI\n");}

    //while ((dup2(piped2[WRITE],STDOUT_FILENO) == -1) && (errno == EINTR)) {printf("HOLI\n");}

    //El hijo lee lo que le llega.
    execl(programa, programa, (char *) 0);
  }else{
    close(piped2[WRITE]);
    close(piped[READ]);
    char a = getIndex(string[0]), b = getIndex(string[2]);
    //close(piped2[WRITE]);
    //int a = 2;
    //write(piped[WRITE], &a, sizeof(int));
    int n = matrices[b].n;
    int m = matrices[b].m;
    int i , j;
    //Se escriben n y m
    write(piped[WRITE], &n, sizeof(int));
    write(piped[WRITE], &m, sizeof(int));
    // Luego se escriben todos los valores de la matriz de forma secuencial
    for (i = 0; i < n; i++) {
      for (j = 0; j < m; j++) {
        int aux = matrices[b].matriz[i][j];
        write(piped[WRITE], &aux, sizeof(int));
      }
    }

    int c = constante;
    printf("Constante 2: %d\n",c);
		write(piped[WRITE], &c, sizeof(int));
    // Finalmente, guarda el valor calculado por el hijo en la variable local.
    int nres, mres;
    read(piped2[READ], &nres, sizeof(int));
    read(piped2[READ], &mres, sizeof(int));
    iniciar_matriz(a, nres, mres);

    for (i = 0; i < nres; i++) {
      for (j = 0; j < mres; j++) {
        int aux;
        read(piped2[READ], &aux, sizeof(int));
        matrices[a].matriz[i][j] = aux;
        //printf("%d\n", aux);
      }
    }

  }

}



// Sacado de stackoverflow
// http://stackoverflow.com/questions/9210528/split-string-with-delimiters-in-c
// Usuario hmjd
// Separa un string a partir de un delimitador
char** str_split(char* a_str, const char a_delim)
{
    char** result    = 0;
    size_t count     = 0;
    char* tmp        = a_str;
    char* last_comma = 0;
    char delim[2];
    delim[0] = a_delim;
    delim[1] = 0;

    /* Count how many elements will be extracted. */
    while (*tmp)
    {
        if (a_delim == *tmp)
        {
            count++;
            last_comma = tmp;
        }
        tmp++;

    }


    /* Add space for trailing token. */
    count += last_comma < (a_str + strlen(a_str) - 1);

    /* Add space for terminating null string so caller
       knows where the list of returned strings ends. */
    count++;

    result = malloc(sizeof(char*) * count);

    if (result)
    {
        size_t idx  = 0;
        char* token = strtok(a_str, delim);

        while (token)
        {
            assert(idx < count);
            *(result + idx++) = strdup(token);
            token = strtok(0, delim);
        }
        assert(idx == count - 1);
        *(result + idx) = 0;
    }

    return result;
}

/*
	Funcion que imprime una matriz cargada en memoria desde el arreglo matrices.
	recibe un caracter con el identificador de la matriz.
*/
int print(char matriz){

    int n = matrices[matriz].n;
    int m = matrices[matriz].m;

    if(n <= 0 || m <= 0){
    	printf("Matriz vacía.\n");
    	return 1;
    }

    int i, j;
    for( i = 0; i < n; i++){
      for ( j = 0; j < m; j++) {
        printf("|  %d  |",matrices[matriz].matriz[i][j]);
      }
      printf("\n");
    }

    return 1;
}

/* Funcion load
** Recibe el identificador de la matriz en memoria a cargar y la dirección del archivo de texto
** Como salida ofrece un codigo, 0 de error, 1 en caso de exito
*/
int load(char mat, char * nombre_archivo){
  char matriz = getIndex(mat);
  //Si se va a cargar una matriz nueva, se sobreescribe lo existente en la memoria
  if(matriz == -1){
    return 0;
  }
  // Comprobado que el id de matriz en memoria es válido, se carga el archivo
  // de texto desde disco.
  FILE *archivo;
  char * buffer;
  char cursor;
  int n = 0, m =0;
  int i= 0, j = 0, k = 0;

  archivo = fopen(nombre_archivo, "r");
  if(archivo == NULL){
    printf("%s\n", nombre_archivo);
    printf("No se puede abrir el archivo\n");
    return 0;
  }
  //Una vez abierto se lee linea por linea
  //fclose(archivo);

  // Se que las primeras dos lineas son los tamaños de la matriz, luego:
  buffer = (char *) malloc(sizeof(char));

  cursor = getc(archivo);
  while(cursor!='\n'){
    buffer[i] = cursor;
    cursor = getc(archivo);
    if(cursor != '\n'){
      i++;
      buffer = realloc(buffer, (i+1)*sizeof(char));
    }
  }

  n = atoi(buffer);
  free(buffer);

  buffer = (char *) malloc(sizeof(char));
  i= 0;
  cursor = getc(archivo);
  while(cursor!='\n'){
    buffer[i] = cursor;
    cursor = getc(archivo);
    if(cursor != '\n'){
      i++;
      buffer = realloc(buffer, (i+1)*sizeof(char));
    }
  }

  m = atoi(buffer);
  free(buffer);

  iniciar_matriz(matriz, n, m);
  // La matriz está inicializada.
  // Luego, para leer la matriz, se itera :
  buffer = (char *) malloc(sizeof(char));
  i = 0;
  cursor = getc(archivo);
  while(cursor != EOF){
    while(cursor!='\n'){
      buffer[i] = cursor;
      cursor = getc(archivo);
      if(cursor != '\n' && cursor != ' '){
        i++;
        buffer = realloc(buffer, (i+1)*sizeof(char));
      }
      else{
        i = 0;
        int numero = atoi(buffer);
        matrices[matriz].matriz[k][j] = numero;
        j++;
        free(buffer);
        buffer = (char *) malloc(sizeof(char));
      }
    }
    j = 0;
    k++;
    cursor = getc(archivo);
  }
  free(buffer);
  print(matriz);
  return 1;
}

/*
	Funcion que copia el contenido de una matriz a otra
	Recibe un caracter con el identificador de la matriz de destino y para la matriz de origen
	Formato comando: B = A

*/
int copiar(char des, char or){

  char destino = getIndex(des), origen = getIndex(or);

  if(destino == -1 || origen == -1){
    return 0;
  }
	//printf("%c, %c\n", origen, destino);
	int n = matrices[origen].n, m = matrices[origen].m;
	//printf("%d, %d\n", n, m);
	iniciar_matriz(destino, n, m);
	//printf("%d, %d\n", matrices[destino].n,  matrices[destino].m);

	int i, j;
	for(i=0; i<n; i++){
		for(j=0; j<m; j++){
			matrices[destino].matriz[i][j] = matrices[origen].matriz[i][j];
		}
	}
	return 1;
}

/*
	Funcion que limpia el contenido de una matriz
	Recibe un caracter con el identificador de la matriz
*/

int clear(char mat){
	char matriz = getIndex(mat);
  if(matriz == -1){
    return 0;
  }
	matrices[matriz].n = 0;
	matrices[matriz].m = 0;
	matrices[matriz].matriz = NULL;

	return 1;
}

/*

	Función que guarda una matriz desde memoria al disco del equipo.
	Recibe el identificador de la matriz como char y el nombre del archivo de destino.

*/
int save(char mat, char * nombre_archivo){
  char matriz = getIndex(mat);
  if(matriz == -1){
    return 0;
  }

  FILE *archivo;
  char * buffer;
  char cursor;
  int n = matrices[matriz].n, m = matrices[matriz].m;
  int i= 0, j = 0, k = 0;

  archivo = fopen(nombre_archivo, "ab+");

  fprintf(archivo, "%d\n%d\n", n, m);

  for(i = 0; i < n; i++){
  	for(j = 0; j <m ; j++){
  		char str[15];
  		sprintf(str, "%d", matrices[matriz].matriz[i][j]);
  		fprintf(archivo, "%s", str);
  		if(j < m-1){
  			fprintf(archivo, " ");
  		}
  	}
  	fprintf(archivo, "\n");
  }
  fclose(archivo);
  return 1;
}

/*
	Función que calcula la matriz transpuesta y guarda el valor en otra matriz.
	Recibe el indetificador de la matriz a operar y la matriz de destino.
*/

int trans(char or, char des){
  char origen = getIndex(or), destino = getIndex(des);
	//Para hacer transpuesta, paso la matriz por pipes y recibo el resultado.
  int piped[2]; // Para escribir en el hijo
  int piped2[2]; // Para leer desde el hijo
  int p1 = pipe(piped);
  int p2 = pipe(piped2);
  if(p1 == -1 || p2 == -1){
    printf("ERROR EN PIPES\n");
  }
  pid_t pid = fork();
  if(pid == 0){
    close(piped2[READ]); // El hijo escribe por el pipe2
    close(piped[WRITE]); // El hijo lee por el pipe1

    DUP2CLOSE(piped[READ],STDIN_FILENO);
    DUP2CLOSE(piped2[WRITE],STDOUT_FILENO);

    execl("./trans", "./trans", (char *) 0);
  }else{
    close(piped2[WRITE]);
    close(piped[READ]);

    //close(piped2[WRITE]);
    //int a = 2;
    //write(piped[WRITE], &a, sizeof(int));
    int n = matrices[origen].n;
    int m = matrices[origen].m;
    int i , j;
    //Se escriben n y m
    write(piped[WRITE], &n, sizeof(int));
    write(piped[WRITE], &m, sizeof(int));
    // Luego se escriben todos los valores de la matriz de forma secuencial
    for (i = 0; i < n; i++) {
      for (j = 0; j < m; j++) {
        int aux = matrices[origen].matriz[i][j];
        write(piped[WRITE], &aux, sizeof(int));
      }
    }
    // Finalmente, guarda el valor calculado por el hijo en la variable local.
    int nres, mres;
    read(piped2[READ], &nres, sizeof(int));
    read(piped2[READ], &mres, sizeof(int));
    iniciar_matriz(destino, nres, mres);

    for (i = 0; i < nres; i++) {
      for (j = 0; j < mres; j++) {
        int aux;
        read(piped2[READ], &aux, sizeof(int));
        matrices[destino].matriz[i][j] = aux;
        //printf("%d\n", aux);
      }
    }

  }
  return 1;
}


int shell(){
  //La shell espera comandos, por lo que se debe recibir un string de texto.
  char *string = NULL;
  size_t len ;
  int go = 1;
  while(go){
    string = NULL;
    printf("Ingresa un comando: \n");
    getline(&string, &len, stdin);
    string[strlen(string)-1] = '\0';


    //Una vez recibido el texto, se debe interpretar:
    //Para eso se obtienen todos los strings recinidos separados por espacios.

    // Teniendo los strings, se analizan:

    //Si el string es único (hay un solo string sin espacios), entonces se debe
    // ejecutar una de las otras ordenes (suma, multiplicacion, etc).
    if(strchr(string, ' ') != NULL){
      //Si posee ' ' (Varios strings)
      char ** strings = str_split(string, ' ');
      if(strings){

        if(!strcmp(strings[0], "load")){
          //Implementado
          int res = load(strings[1][0], strings[2]);
          if(res == 1){
            printf("Matriz cargada exitosamente en memoria.\n");
          }else{
            printf("Ocurrió un error al cargar la matriz.\n");
          }
        }else if(!strcmp(strings[0], "save")){
      	  int res = save(strings[1][0], strings[2]);
					if(res == 1){
						printf("Matriz guardada exitosamente en disco.\n");
					}else{
						printf("Ocurrió un error al guardar la matriz.\n");
					}
        }else if(!strcmp(strings[0], "print")){
          char index = getIndex(strings[1][0]);
          print(index);
        }else if(!strcmp(strings[0], "clear")){
          int res = clear(strings[1][0]);
          if(res == 1){
          	printf("Matriz limpiada.\n");
          }else{
          	printf("Error en la operación.");
          }
        }else if(!strcmp(strings[1], "=")){
          int res = copiar(strings[0][0], strings[2][0]);
          if(res == 1){
          	printf("Matriz copiada. \n");
          }else{
          	printf("Operación fallida. No se han podido copiar las matrices. Revise sus datos.\n");
          }
        }else if(strings[0][2] == 't'){
        	//Transpuesta
        	int res = trans(strings[1][0], strings[0][0]);
        	if(res == 1){
        		printf("Matriz transpuesta correctamente.\n");
        	}else{
        		printf("Error al transponer matriz.\n");
        	}
        }else{
          printf("Opción no válida.\n");
        }

        free(strings);
      }

    }else{
      //Un string
      if(!strcmp(string, "exit")){
        printf("Salir\n");
        return 1;
      }
      /*
      int pid = fork();
      if(pid == 0){
        execl("./s", "./s", (char *) 0);
      }*/
      //El resto de comandos de un string generan un nuevo proceso con fork y
      // exec.
      //Se debe decidir a dónde enviar dependiendo del formato.
      if(string[3] == '+'){
        printf("Hacer suma\n");
        operar(string, "./s");
        //Comprobar valores:
        //printf("%c = %d\n", string[0], string[0] - 65);
      }else if(string[3] == '-'){
        printf("Hacer resta\n");
				operar(string, "./r");
      }else if(string[3] == '*'){
				if(string[4] >= 65 && string[4] <= 90){
        	printf("Hacer multiplicacion por matriz\n");
					operar(string, "./mul");
				}else{
					//Multiplicar por constante.
					int largo_string = strlen(string);
					int index = 0, index2;
					char numero_string[15];
					for(index = 4; index < largo_string; index++){
						if(index >= 15){
							break;
						}
						numero_string[index - 4] = string[index];
					}
					//Con la copia del string se intenta pasar a numero
					//En caso de que se ingresen datos no numericos, atoi retorna 0 por defecto
					int numero = atoi(numero_string);
					printf("Constante: %d\n", numero);
					operar2(string, "./mulc", numero);
				}
      }else{
        printf("Opción no válida.\n");
      }
    }
  }



}

int main(int argc, char * argv[]){
  // El padre recibe comandos mediante una shell.
  // La shell se encarga de redirigir las acciones en base a los comandos
  matrices = (matriz *)malloc(26*sizeof(matriz));

  shell();

  free(matrices);
}
