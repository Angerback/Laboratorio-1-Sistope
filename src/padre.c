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
    int n = matrices[string[2]].n;
    int m = matrices[string[2]].m;
    int i , j;
    //Se escriben n y m
    write(piped[WRITE], &n, sizeof(int));
    write(piped[WRITE], &m, sizeof(int));
    // Luego se escriben todos los valores de la matriz de forma secuencial
    for (i = 0; i < n; i++) {
      for (j = 0; j < m; j++) {
        int aux = matrices[string[2]].matriz[i][j];
        write(piped[WRITE], &aux, sizeof(int));
      }
    }
    //Se pasa la otra matriz:
    n = matrices[string[4]].n;
    m = matrices[string[4]].m;

    write(piped[WRITE], &n, sizeof(int));
    write(piped[WRITE], &m, sizeof(int));
    // Luego se escriben todos los valores de la matriz de forma secuencial
    for (i = 0; i < n; i++) {
      for (j = 0; j < m; j++) {
        int aux = matrices[string[4]].matriz[i][j];
        write(piped[WRITE], &aux, sizeof(int));
      }
    }
    // Finalmente, guarda el valor calculado por el hijo en la variable local.
    int nres, mres;
    read(piped2[READ], &nres, sizeof(int));
    read(piped2[READ], &mres, sizeof(int));
    iniciar_matriz(string[0], nres, mres);

    for (i = 0; i < nres; i++) {
      for (j = 0; j < mres; j++) {
        int aux;
        read(piped2[READ], &aux, sizeof(int));
        matrices[string[0]].matriz[i][j] = aux;
        //printf("%d\n", aux);
      }
    }

  }

}


// Sacado de stackoverflow
// http://stackoverflow.com/questions/9210528/split-string-with-delimiters-in-c
// Usuario hmjd
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

int print(char matriz){
    if(matriz < 65 || matriz > 90){
      return 0;
    }
    int n = matrices[matriz].n;
    int m = matrices[matriz].m;
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
int load(char matriz, char * nombre_archivo){
  if(matriz < 65 || matriz > 90){
    return 0;
  }
  //Si se va a cargar una matriz nueva, se sobreescribe lo existente en la memoria

  // Comprobado que el id de matriz en memoria es válido, se carga el archivo
  // de texto desde disco.
  FILE *archivo;
  archivo = fopen(nombre_archivo, "r");

  if(archivo == NULL){
    printf("%s\n", nombre_archivo);
    printf("No se puede abrir el archivo\n");
    return 0;
  }
  //Una vez abierto se lee linea por linea
  //fclose(archivo);
  char * buffer;
  char cursor;
  int n = 0, m =0;
  // Se que las primeras dos lineas son los tamaños de la matriz, luego:
  buffer = (char *) malloc(sizeof(char));
  int i= 0;
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
  int j = 0, k = 0; // Usados para moverse en la matriz.
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

int shell(){
  //La shell espera comandos, por lo que se debe recibir un string de texto.
  char *string = NULL;
  size_t len ;
  int go = 1;
  while(go){
    string = NULL;
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
          printf("Se hace load!\n");
          int res = load(strings[1][0], strings[2]);
          if(res == 1){
            printf("Matriz cargada exitosamente en memoria.\n");
          }else{
            printf("Ocurrió un error al cargar la matriz.\n");
          }
        }else if(!strcmp(strings[0], "save")){
          printf("Se hace save!\n");
        }else if(!strcmp(strings[0], "print")){
          printf("Print\n" );
          print(strings[1][0]);
        }else if(!strcmp(strings[0], "clear")){
          printf("Clear\n" );
        }else if(!strcmp(strings[1], "=")){
          printf("Igualación\n" );
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
      }else if(string[3] == '*'){
        printf("Hacer multiplicacion\n");
      }else if(string[3] == 'r'){
        printf("Hacer trans\n");
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
