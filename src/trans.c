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
  int i = 0, j =0, n, n2, m2, m, aux;
  int ** mat1;
  int ** mat2;
  int ** resultado;

  //Matriz transpuesta
  //Se leen los valores dejados en el pipe para n y m
  read(STDIN_FILENO, &n, sizeof(int));
  read(STDIN_FILENO, &m, sizeof(int));
  mat1 = (int **)malloc(n * sizeof(int*));
  for(i = 0; i < n; i++){
    mat1[i] = (int *)malloc(m * sizeof(int));
  }
  // se lee secuencialmente el valor para cada casilla de la mat1riz
  for ( i = 0; i < n; i++) {
    for (j = 0; j < m; j++) {
      read(STDIN_FILENO, &aux, sizeof(int));
      mat1[i][j] = aux;
    }

  }

  resultado = (int **)malloc(m * sizeof(int*));
  for(i = 0; i < m; i++){
    resultado[i] = (int *)malloc(n * sizeof(int));
  }
  //Se transpone la matriz
  for ( i = 0; i < n; i++) {
    for (j = 0; j < m; j++) {
      resultado[j][i] = mat1[i][j];
    }
  }

  n2 = m;
  m2 = n;

  // Se envian los datos de la matriz a través del pipe:
  aux = n2;
  write(STDOUT_FILENO, &aux, sizeof(int));
  aux = m2;
  write(STDOUT_FILENO, &aux, sizeof(int));
  //Y se envía el resultado al padre:
  for ( i = 0; i < n2; i++) {
    for (j = 0; j < m2; j++) {
      aux = resultado[i][j];
      write(STDOUT_FILENO, &aux, sizeof(int));
    }

  }
}
