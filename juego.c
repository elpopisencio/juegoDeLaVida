#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
   FILE *f;
   char *s, *res;
   int rows, cols, steps, n, i, j, ncols, nrows;

	 /*************************************************
		 Definición de "old" y resto de variables
		**************************************************/
   
   // Modificado por mi
   int **old;
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
   old = (int **)malloc((rows + 2)*sizeof(int*)); 
	
   for (i=0;i<(rows + 2);i++) 
     old[i] = (int*)malloc((cols + 2)*sizeof(int));
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

   for (i = 0; i < rows + 2; i++){
     for (j = 0; j < cols + 2; j++){
       printf("%d ", old[i][j]);
     }
     printf("\n");
   }
}
