// Integrantes: Beroisa Jorge, Gutierrez Jeremias.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <emmintrin.h>
#include <omp.h>


void viejo(char** old, char** new, int rows, int cols){
  int i, j, suma;



  for (j = 1; j < cols + 1; j++){
    old[0][j] = old[rows][j];
    old[rows + 1][j] = old[1][j];
  }
  for (i = 1; i < rows + 1; i++){
    old[i][0] = old[i][cols];
    old[i][cols + 1] = old[i][1];
  }
  old[0][0] = old[rows][cols];
  old[0][cols + 1] = old[rows][1];
  old[rows + 1][0] = old[1][cols];
  old[rows + 1][cols + 1] = old[1][1];
  
  for (i = 0; i < rows + 2; i++){
    for (j = 0; j < cols + 2; j++){
      if(i != 0 && j != 0 && i != rows + 1 && j != cols + 1){
	suma = 0;
	suma += old[i-1][j-1];
	suma += old[i-1][j];
	suma += old[i][j-1];
	suma += old[i+1][j+1];
	suma += old[i+1][j];
	suma += old[i][j+1];
	suma += old[i+1][j-1];
	suma += old[i-1][j+1];
	  
	if (suma == 3){
	  new[i][j] = 1;
	}else{
	  if (old[i][j] == 1 && suma == 2){
	    new[i][j] = 1;
	  } else{
	    new[i][j] = 0;
	  }
	}
      } else{
	new[i][j] = 0;
      }
    }     
  }

}




void actualizar(char** old, int** viv2, int rows, int cols, int i, int j){
  int suma = 0;

  if(i==0 || j==0 || i == rows+1 || j==cols+1){
    if(i == 0){
      i = rows;
    }else{
      if(i == rows + 1){
	i = 1;
      }
    }
    if(j == 0){
      j = cols;
    }else{
      if(j == cols + 1){
	j = 1;
      }
    }
  }
  
  if((old[i][j] == 1 || old[i][j] == 0)){

    suma += old[i-1][j-1] & 1;
    suma += old[i-1][j] & 1;
    suma += old[i-1][j+1] & 1;
  
    suma += old[i][j-1] & 1;
    suma += old[i][j+1] & 1;

    suma += old[i+1][j-1] & 1;
    suma += old[i+1][j] & 1;
    suma += old[i+1][j+1] & 1;
     
    if (suma == 3 || (suma == 2 && old[i][j]  == 1)){
       viv2[i][ viv2[i][0] + 1 ] = j;
      viv2[i][0]++;
      old[i][j] = old[i][j] | 2;
    } 
    old[i][j] = old[i][j] | 2;
  }
 
}

void mejor(char** old, int** viv1, int** viv2, int rows, int cols){
  int i, j;    

#pragma omp parallel for  schedule (static)
  
  for (j = 1; j < cols + 1; j++){
    old[0][j] = old[rows][j];
    old[rows + 1][j] = old[1][j];
  }

  #pragma omp parallel for  schedule (static)
  
  for (i = 1; i < rows + 1; i++){
    old[i][0] = old[i][cols];
    old[i][cols + 1] = old[i][1];
  }
  old[0][0] = old[rows][cols];
  old[0][cols + 1] = old[rows][1];
  old[rows + 1][0] = old[1][cols];
  old[rows + 1][cols + 1] = old[1][1];

  #pragma omp parallel for  schedule (static)
  for(i = 1; i <= rows; i++){
    viv2[i][0] = 0;
  }

#pragma omp parallel for private (j) schedule (static)
  
  for(i = 1; i <= rows; i++){  
    for(j = 1; j <= viv1[i][0]; j++){
      actualizar(old, viv2, rows, cols, i-1, viv1[i][j]-1);
      actualizar(old, viv2, rows, cols, i-1, viv1[i][j]);
      actualizar(old, viv2, rows, cols, i-1, viv1[i][j]+1);
    
      actualizar(old, viv2, rows, cols, i, viv1[i][j]-1);
      actualizar(old, viv2, rows, cols, i, viv1[i][j]);
      actualizar(old, viv2, rows, cols, i, viv1[i][j]+1);

      actualizar(old, viv2, rows, cols, i+1, viv1[i][j]-1);
      actualizar(old, viv2, rows, cols, i+1, viv1[i][j]);
      actualizar(old, viv2, rows, cols, i+1, viv1[i][j]+1);
    }
 
  }
  
  /*
    for (i = 1; i <= rows + 1; i++){
      for (j = 0; j <= cols + 1; j++){
	printf("%d  ",  viv2[i][j]);
      }
      printf("\n");
    }
 */
 #pragma omp parallel for  schedule (static)
  for(j = 1; j <= cols; j++){
    old[1][j] = 0;
    old[rows][j] = 0;
  }

#pragma omp parallel for private (j) schedule (static)
  for(i = 1; i <= rows; i++){  
    old[i][1] = 0;
    old[i][cols] = 0;
    for(j = 1; j <= viv1[i][0]; j++){
      old[ i - 1 ][ viv1[i][j] - 1 ] = 0;
      old[ i - 1 ][ viv1[i][j] ] = 0;
      old[ i - 1 ][ viv1[i][j] + 1] = 0;
    
      old[ i ][ viv1[i][j] - 1] = 0;
      old[ i ][ viv1[i][j] ] = 0;
      old[ i ][ viv1[i][j] + 1] = 0;

      old[ i + 1 ][ viv1[i][j] - 1 ] = 0;
      old[ i + 1 ][ viv1[i][j] ] = 0;
      old[ i + 1 ][ viv1[i][j] + 1 ] = 0;
    }
  }
  /*
      printf("\n");
  for (i = 1; i <= rows + 1; i++){
      for (j = 1; j <= cols + 1; j++){
	printf("%d  ",  old[i][j]);
      }
      printf("\n");
    }
  */

#pragma omp parallel for private(j) schedule (static)
  for(i = 1; i <= rows; i++){  
    for(j = 1; j <= viv2[i][0]; j++){
      old[ i ][ viv2[i][j] ] = 1;
    }
  }
}


int main(int argc, char *argv[]) {
  FILE *f;
  char *s, *res;
  int rows, cols, steps, n, i, j, ncols, nrows;

  /*************************************************
		 Definición de "old" y resto de variables
  **************************************************/
   
  // Modificado por mi
  char **old;
  char **new;
  char **aux1;
  int **viv1;
  int **viv2;
  int **aux;
  int a;
  //fin modificado
   
  /*  Controla que se haya ingresado un argumento en la llamada */
  if (argc != 2){ 
    printf("Error: Debe indicar el nombre del archivo de entrada\nEj: $./a.out entrada1.cells\n");
    return 1;
  }

  /*  Lectura del encabezado del archivo que contiene el patron de celdas */
  f = fopen(argv[1],"r");
  if (f == NULL) {
    printf("Error al intentar abrir el archivo.\n");
    return 1;
  }
  n = fscanf(f, "cols %d\nrows %d\nsteps %d\n", &cols, &rows, &steps);
  if (n != 3)  {
    printf("Error: formato de archivo incorrecto\n");
    return 1;
  }
  printf("cols %d\nrows %d\nsteps %d\n", cols, rows, steps);

  /*************************************************
		 Aquí debería asignarse memoria a las matrices
 		 ...
		 ...
        
  **************************************************/
  // Modificado por mi
  old = malloc((rows + 2)*sizeof(char*));
  new = malloc((rows + 2)*sizeof(char*));
  viv1 = malloc((rows + 2)*sizeof(int*));
  viv2 = malloc((rows + 2)*sizeof(int*));
  
  if(old != NULL && viv1 != NULL && viv2 != NULL ){
    for (i=0;i<(rows + 2);i++){ 
      old[i] = malloc((cols + 2)*sizeof(char));
      new[i] = malloc((cols + 2)*sizeof(char));
      viv1[i] = malloc((cols + 2)*sizeof(int));
      viv2[i] = malloc((cols + 2)*sizeof(int));
      
      if(old[i] == NULL && viv1[i] == NULL && viv2[i] == NULL){
	printf("No hay memoria");
	return 1;
      }
    }
  } else {
    printf("No hay memoria");
    return 1;
  }
  
  nrows = (rows + 2) * 2;
  ncols = cols;
        
  //fin modificado
   

  /*  inicializar elementos con 0 o 1 en matriz llamada "old" */
  s = malloc(cols + 1);
  res = fgets(s, cols + 2, f);
 


  for(i = 1; i <= rows; i++){
    viv1[i][0] = 0;
  }
  i = 1;
  
  while (res != NULL) {
    
    for (j = 0; j < (int)strlen(s) - 1; j++)
      if(s[j] == '.'){
	old[i][j+1] = 0;
      } else{
	old[i][j+1] = 1;
	
	viv1[i][ viv1[i][0]+1 ] = j + 1;
	viv1[i][0]++;
      }
    for (j = strlen(s) - 1; j < cols; j++)
      old[i][j+1] = 0;
    res = fgets(s, cols, f);
    i++;
  
  };
  
  
  for (j = i; j < rows + 2; j++)
    bzero(old[j], ncols);
  fclose(f);
  free(s);
  for(i=0; i<rows+1; i++)
    bzero(new[i], ncols);
  /*************************************************
		 Resto del programa
 		 ...
		 ...
        
  **************************************************/
 
  
  for(a = 0; a < steps; a++){
    
    // viejo(old, new, rows, cols);
    mejor(old, viv1, viv2, rows, cols);


    /*   
    for(i=1; i<rows+1; i++){
      for(j=1; j<cols+1; j++){
	  if(old[i][j] != new[i][j]){
	    printf("%d, %d, %d \n", i, j, a);
	    printf("%d",old[i-1][j-1]);
	    printf("%d",old[i-1][j]);
	    printf("%d",old[i-1][j+1]);
	          printf("\n");
	    printf("%d",old[i][j-1]);
	    printf("%d",old[i][j]);
	    printf("%d", old[i][j+1]);
      printf("\n");
	    printf("%d",old[i+1][j-1]);
	    printf("%d",old[i+1][j]);
	    printf("%d",old[i+1][j+1]);
	        printf("\n");
	   printf("%d",new[i-1][j-1]);
	    printf("%d",new[i-1][j]);
	    printf("%d",new[i-1][j+1]);
	          printf("\n");
	    printf("%d",new[i][j-1]);
	    printf("%d",new[i][j]);
	    printf("%d", new[i][j+1]);
      printf("\n");
	    printf("%d",new[i+1][j-1]);
	    printf("%d",new[i+1][j]);
	    printf("%d",new[i+1][j+1]);
	        printf("\n");
	    return 1;
	  }
	}
	    }
        
    aux1 = old;
    old = new;
    new = aux1;
*/
    
    aux = viv1;
    viv1 = viv2;
    viv2 = aux;
 
} 

  
    for (i = 1; i < rows + 1; i++){
      for (j = 1; j < cols + 1; j++){
	printf("%c",  (old[i][j] == 1) ? 'O' : '.');
      }
      printf("\n");
    }

  
  return 0;
}
