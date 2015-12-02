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

  //MULTIPLICAION DE MATRICES

  int i = 0, j =0;


  //SE RECIBE LA PRIMERA MATRIZ A MULTIPLICAR
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
  // se lee secuencialmente el valor para cada casilla de la mat1riz
  for ( i = 0; i < n; i++) {
    for (j = 0; j < m; j++) {
      read(STDIN_FILENO, &aux, sizeof(int));
      mat1[i][j] = aux;
    }

  }

  i = 0;

  //SE RECIBEN LOS VALORES PARA LA SEGUNDA MATRIZ QUE SE MULTIPLICARA
  int n2;
  int m2;
  //Se leen los valores dejados en el pipe para n y m
  read(STDIN_FILENO, &n2, sizeof(int));
  read(STDIN_FILENO, &m2, sizeof(int));
  int ** mat2;
  mat2 = (int **)malloc(n2 * sizeof(int*));
  for(i = 0; i < n2; i++){
    mat2[i] = (int *)malloc(m2 * sizeof(int));
  }
  // se lee secuencialmente el valor para cada casilla de la mat1riz
  for ( i = 0; i < n2; i++) {
    for (j = 0; j < m2; j++) {
      read(STDIN_FILENO, &aux, sizeof(int));
      mat2[i][j] = aux;
    }

  }


  int ** resultado;
  resultado = (int **)malloc(n * sizeof(int*));
  for(i = 0; i < n; i++){
    resultado[i] = (int *)malloc(m * sizeof(int));
  }
  //Se multiplican las matrices 
  //Obervacion: hay que comprobar que las matrices puedan ser multiplicables. Probe este algoritmo con matrices que si funcionan
  for ( i = 0; i < n; i++) {
    for (j = 0; j < m2; j++) {
      resultado[i][j] = 0;
      int k ;
      for ( k = 0; k < m; k++)
      {
        resultado[i][j] = (resultado[i][j] + (mat1[i][k]*mat2[k][j]));
      }
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
