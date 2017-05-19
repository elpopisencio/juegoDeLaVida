#include <stdio.h>
#include <stdlib.h>
#include <string.h>


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


void mejorado(char** old, char** new, int rows, int cols){
  int i, j;

  // Recorro la primer fila:

  if (old[1][1] == 1){
    new[rows][cols] += 1;
    new[rows][1] += 1;
    new[rows][2] += 1;

    new[1][cols] += 1;
    new[1][2] += 1;
	
    new[2][cols] += 1;
    new[2][1] += 1;
    new[2][2] += 1;
  }
  
  for (j = 2; j < cols; j++){
 
    if (old[1][j] == 1){
      new[rows][j-1] += 1;
      new[rows][j] += 1;
      new[rows][j+1] += 1;

      new[1][j-1] += 1;
      new[1][j+1] += 1;
	
      new[2][j-1] += 1;
      new[2][j] += 1;
      new[2][j+1] += 1;
    }
  }

  if (old[1][cols] == 1){
    new[rows][cols - 1] += 1;
    new[rows][cols] += 1;
    new[rows][1] += 1;

    new[1][cols - 1] += 1;
    new[1][1] += 1;
	
    new[2][cols-1] += 1;
    new[2][cols] += 1;
    new[2][1] += 1;
  }
  
  // Recorro las filas intermedias:
  for (i = 2; i < rows; i++){
    // Verifico el primer elemento de la fila:
    if (old[i][1] == 1){
      new[i-1][cols] += 1;
      new[i-1][1] += 1;      
      new[i-1][2] += 1;
	
      new[i][cols] += 1;
      new[i][2] += 1;

      new[i+1][cols] += 1;
      new[i+1][1] += 1;
      new[i+1][2] += 1;
    
    }

    // Verifico los intermedios:
    for (j = 2; j < cols; j++){
 
      if (old[i][j] == 1){
	new[i-1][j-1] += 1;
	new[i-1][j] += 1;
	new[i-1][j+1] += 1;
	
	new[i][j-1] += 1;
	new[i][j+1] += 1;

	new[i+1][j-1] += 1;	
	new[i+1][j] += 1;
	new[i+1][j+1] += 1;
      }
    }
    // Verifico el ultimo elemento de la fila:
    if (old[i][cols] == 1){
      new[i-1][cols - 1] += 1;
      new[i-1][cols] += 1;      
      new[i-1][1] += 1;

      new[i][cols - 1] += 1;
      new[i][1] += 1;

      new[i+1][cols - 1] += 1;
      new[i+1][cols] += 1;
      new[i+1][1] += 1;
    } 
    
  }


  // Recorro la ultima fila:

  if (old[rows][1] == 1){
    new[rows - 1][cols] += 1;
    new[rows - 1][1] += 1;
    new[rows - 1][2] += 1;

    new[rows][cols] += 1;
    new[rows][2] += 1;
	
    new[1][cols] += 1;
    new[1][1] += 1;
    new[1][2] += 1;
  }

  for (j = 2; j < cols; j++){
    if (old[rows][j] == 1){
      new[rows - 1][j-1] += 1;
      new[rows - 1][j] += 1;
      new[rows - 1][j+1] += 1;

      new[rows][j-1] += 1;
      new[rows][j+1] += 1;
	
      new[1][j-1] += 1;
      new[1][j] += 1;
      new[1][j+1] += 1;
    }
  }

  if (old[rows][cols] == 1){
    new[rows - 1][cols-1] += 1;
    new[rows - 1][cols] += 1;
    new[rows - 1][1] += 1;

    new[rows][cols-1] += 1;
    new[rows][1] += 1;
	
    new[1][cols-1] += 1;
    new[1][cols] += 1;
    new[1][1] += 1;
  }



  for (i = 1; i < rows + 1; i++){
    for (j = 1; j < cols + 1; j++){
    
      if (new[i][j] == 3){
	new[i][j] = 1;
	old[i][j] = 0;
      } else{
	if (new[i][j] == 2 && old[i][j] == 1){
	  new[i][j] = 1;
	  old[i][j] = 0;
	} else{
	  new[i][j] = 0;
	  old[i][j] = 0;
	}
      }
    
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
  int suma;
  char **oldv;
  char **oldm;
  char **newv;
  char **newm;
  char **aux;
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
  oldv = malloc(((rows + 2) * 2)*sizeof(char*)); 

  if(oldv != NULL){
    for (i=0;i<((rows + 2) * 2);i++){ 
      oldv[i] = malloc((cols + 2)*sizeof(char));
      if(oldv[i] == NULL){
	printf("No hay memoria");
	return 1;
      }
    }
  } else {
    printf("No hay memoria");
    return 1;
  }

  oldm = malloc(((rows + 2) * 2)*sizeof(char*)); 

  if(oldm != NULL){
    for (i=0;i<((rows + 2) * 2);i++){ 
      oldm[i] = malloc((cols + 2)*sizeof(char));
      if(oldm[i] == NULL){
	printf("No hay memoria");
	return 1;
      }
    }
  } else {
    printf("No hay memoria");
    return 1;
  }


  
  newv = oldv + rows + 2;
  newm = oldm + rows + 2;
  nrows = (rows + 2) * 2;
  ncols = cols;
        
  //fin modificado
  

  /*  inicializar elementos con 0 o 1 en matriz llamada "old" */
  s = malloc(cols + 1);
  res = fgets(s, cols, f);
  i = 1;
  while (res != NULL) {
    for (j = 0; j < strlen(s) - 1; j++)
      oldm[i][j+1] = (s[j] == '.') ? 0 : 1;
    for (j = strlen(s) - 1; j < cols; j++)
      oldm[i][j+1] = 0;
    res = fgets(s, cols, f);
    i++;
  };
  for (j = i; j < nrows; j++)
    bzero(oldm[j], ncols);
  fclose(f);
  free(s);

  
  for (i = 1; i < rows + 1; i++){
    for (j = 1; j < cols + 1; j++){
      oldv[i][j] = oldm[i][j]; 
    }
  }  
  
  /*************************************************
		 Resto del programa
 		 ...
		 ...
        
  **************************************************/

  /*
  for (i = 0; i < rows + 2; i++){
    for (j = 0; j < cols + 2; j++){
      printf("%c",  (oldv[i][j] == 0) ? '.' : 'O');
    }
     
    printf("\n");
  }

  */
  
  
  for(a = 0; a < steps; a++){
  
    viejo(oldv, newv, rows, cols);
    
      mejorado(oldm, newm, rows, cols);
      for (i = 1; i < rows + 1; i++){
      for (j = 1; j < cols + 1; j++){
      if (newm[i][j] != newv[i][j]){
      printf("super error en el ciclo  %d \nEn la fila: %d, columna %d \n", a, i, j);
      return 1;
      }
      }
      }
      aux = newm;
      newm = oldm;
      oldm = aux;
    
    for (i = 1; i < rows + 1; i++){
      for (j = 1; j < cols + 1; j++){
	printf("%c",  (newv[i][j] == 0) ? '.' : 'O');
      }
     
      printf("\n");
    }
    aux = newv;
    newv = oldv;
    oldv = aux; 
  }
  
  if (steps % 2 == 1){

    aux = newm;
    newm = oldm;
    oldm = aux;    
  }

  
  for (i = 1; i < rows + 1; i++){
    for (j = 1; j < cols + 1; j++){
      printf("%c",  (newv[i][j] == 0) ? '.' : 'O');
    }
     
    printf("\n");
  }
  
}
