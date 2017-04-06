// Integrantes: Beroisa Jorge, Gutierrez Jeremias.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void mejorado(char** old, char** new, int rows, int cols){
  int i, j;
  /* Este metodo recorre la matriz old buscando celulas vivas, cuando
     encuentra una, suma uno a cada celula vecina en la matriz new, que debe
     ser ingresada con todos sus valores en 0.
     Luego, recorre la matriz verificando si la cantidad de celulas vivas
     vecinas y el estado en la matriz old, hace que se mantengan vivas en el 
     nuevo estado, que es guardado en la matriz new, mientras que la matriz old
     queda con todos sus elementos en 0.
   */
  
  // Recorro la primer fila:
  // Verifico el primer elemento de la primer fila:
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
  // Verifico los elementos intermedios
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
  // Verifico el ultimo elemento de la primer fila
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
    // Verifico el primer elemento de la fila i:
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

    // Verifico los intermedios de la fila i:
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
    // Verifico el ultimo elemento de la fila i:
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
  // verifico el primer elemento de la ultima fila:
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
  //verifico los elementos intermedios de la ultima fila:
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
  // Verifico el ultimo elemento de la ultima fila:
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
  // Recorro la matriz new que contiene la cantidad de vecinos de cada celula
  // en el estado anterior y actualizo si debe vivir o morir en el nuevo estado
  for (i = 1; i < rows + 1; i++){
    for (j = 1; j < cols + 1; j++){
      if (new[i][j] == 3 || (new[i][j] == 2 && old[i][j] == 1)){
	new[i][j] = 1;
	old[i][j] = 0;
      } else{
	  new[i][j] = 0;
	  old[i][j] = 0;
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
  char **old;
  char **new;
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
  old = malloc(((rows + 2) * 2)*sizeof(char*)); 

  if(old != NULL){
    for (i=0;i<((rows + 2) * 2);i++){ 
      old[i] = malloc((cols + 2)*sizeof(char));
      if(old[i] == NULL){
	printf("No hay memoria");
	return 1;
      }
    }
  } else {
    printf("No hay memoria");
    return 1;
  }

  new = old + rows + 2;
  nrows = (rows + 2) * 2;
  ncols = cols;
        
  //fin modificado
   

  /*  inicializar elementos con 0 o 1 en matriz llamada "old" */
  s = malloc(cols + 1);
  res = fgets(s, cols + 2, f);
  i = 1;
  while (res != NULL) {
    for (j = 0; j < strlen(s) - 1; j++)
      old[i][j+1] = (s[j] == '.') ? 0 : 1;
    for (j = strlen(s) - 1; j < cols; j++)
      old[i][j+1] = 0;
    res = fgets(s, cols, f);
    i++;
  };
  for (j = i; j < nrows; j++)
    bzero(old[j], ncols);
  fclose(f);
  free(s);
  /*************************************************
		 Resto del programa
 		 ...
		 ...
        
  **************************************************/
  for(a = 0; a < steps; a++){
    mejorado(old, new, rows, cols);
    aux = new;
    new = old;
    old = aux;
  }
  for (i = 1; i < rows + 1; i++){
    for (j = 1; j < cols + 1; j++){
      printf("%c",  (old[i][j] == 0) ? '.' : 'O');
    }
    printf("\n");
  }
}
