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
  nrows = rows + 2;
  ncols = cols + 2;
        
  //fin modificado
   
   /*  inicializar elementos con 0 o 1 en matriz llamada "old" */
   s = malloc(cols + 1);
   res = fgets(s, cols, f);
   i = 1;
   while (res != NULL) {
      for (j = 0; j < strlen(s) - 1; j++)
         old[i][j+1] = (s[j] == '.') ? 0 : 1;
      for (j = strlen(s) - 1; j < cols; j++)
         old[i][j+1] = 0;
      res = fgets(s, cols, f);
      i++;
   };
   for (j = i; j < nrows - 1; j++)
      bzero(old[j], ncols);
   fclose(f);
   free(s);

  
  /*************************************************
		 Resto del programa
 		 ...
		 ...
        
  **************************************************/
  


   
  for(a = 0; a < steps; a++){

    viejo(old, new, rows, cols);
  
    aux = new;
    new = old;
    old = aux;
  }
 
  if (steps % 2 == 1){

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
