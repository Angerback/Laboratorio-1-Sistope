#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "matriz.h"

int main(int argc, char * argv[]){
  

  int i = 0, j =0;

  //Multiplicaion de una matriz por una constante



//tomo la matriz que esta pasando por los pipes
  int n;
  int m;
  //Se leen los valores dejados en el pipe para n y m
  read(STDIN_FILENO, &n, sizeof(int));
  read(STDIN_FILENO, &m, sizeof(int));
  int ** mat1;
  mat1 = (int **)malloc(n * sizeof(int*));
  for(i = 0; i < n; i++){
    mat1[i] = (int *)malloc(m * sizeof(int));
  }
  int aux;
  // se lee secuencialmente el valor para cada casilla de la matriz
  for ( i = 0; i < n; i++) {
    for (j = 0; j < m; j++) {
      read(STDIN_FILENO, &aux, sizeof(int));
      mat1[i][j] = aux;
    }

  }


 /*int numero;
 read(STDIN_FILENO,&numero, sizeof(int));
 */
 int valor;
 read(STDIN_FILENO, &aux, sizeof(int));
 valor = aux;


  int ** resultado;
  resultado = (int **)malloc(n * sizeof(int*));
  for(i = 0; i < n; i++){
    resultado[i] = (int *)malloc(m * sizeof(int));
  }
  //Se multiplica la matriz 
  for ( i = 0; i < n; i++) {
    for (j = 0; j < m; j++) {
      resultado[i][j] = mat1[i][j] * valor;
    }
  }


  aux = n;
  write(STDOUT_FILENO, &aux, sizeof(int));
  aux = m;
  write(STDOUT_FILENO, &aux, sizeof(int));
  //Y se envía el resultado al padre:
  for ( i = 0; i < n; i++) {
    for (j = 0; j < m; j++) {
      aux = resultado[i][j];
      write(STDOUT_FILENO, &aux, sizeof(int));
    }

  }


}
