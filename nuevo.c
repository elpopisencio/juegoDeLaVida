// Integrantes: Beroisa Jorge, Gutierrez Jeremias.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <emmintrin.h>

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
  __m128i res, ressig, f1, f2, f2sig, f3, aux1, aux2;
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

  for (i = 1; i < rows + 1; i++){
    f1 = _mm_load_si128((__m128i*)(old[i-1]+0));
    f2 = _mm_load_si128((__m128i*)(old[i]+0));
    f3 = _mm_load_si128((__m128i*)(old[i+1]+0));
    res = _mm_add_epi8 (f1, f2);
    res = _mm_add_epi8 (res, f3);
    //siguiente
    f1 = _mm_load_si128((__m128i*)(old[i-1]+1));
    f2 = _mm_load_si128((__m128i*)(old[i]+1));
    f3 = _mm_load_si128((__m128i*)(old[i+1]+1));
    ressig = _mm_add_epi8 (f1, f2);
    ressig = _mm_add_epi8 (res, f3);
    
    aux1 =  _mm_slli_si128 (res, 1);
    res = _mm_add_epi8 (res, aux1);
    aux1 = _mm_srli_si128 (ressig, 15);
    
    aux2 =  _mm_srli_si128 (res, 1);


    
   
    res = _mm_add_epi8 (res, aux2);
    res = _mm_sub_epi8 (res, f2);
    _mm_store_si128((__m128i*)(new[i]+0), res);
    for (j = 1; j < cols -16; j = j + 16){
      f1 = _mm_load_si128((__m128i*)(old[i-1]+j));
      f2 = _mm_load_si128((__m128i*)(old[i]+j));
      f3 = _mm_load_si128((__m128i*)(old[i+1]+j));
      res = _mm_add_epi8 (f1, f2);
      res = _mm_add_epi8 (res, f3);
      aux1 =  _mm_slli_si128 (res, 1);
      aux2 =  _mm_srli_si128 (res, 1);
      res = _mm_add_epi8 (res, aux1);
      res = _mm_add_epi8 (res, aux2);
      res = _mm_sub_epi8 (res, f2);
      _mm_store_si128((__m128i*)(new[i]+j), res);
    }
    f1 = _mm_load_si128((__m128i*)(old[i-1]+j));
      f2 = _mm_load_si128((__m128i*)(old[i]+j));
      f3 = _mm_load_si128((__m128i*)(old[i+1]+j));
      res = _mm_add_epi8 (f1, f2);
      res = _mm_add_epi8 (res, f3);
      aux1 =  _mm_slli_si128 (res, 1);
      aux2 =  _mm_srli_si128 (res, 1);
      res = _mm_add_epi8 (res, aux1);
      res = _mm_add_epi8 (res, aux2);
      res = _mm_sub_epi8 (res, f2);
      _mm_store_si128((__m128i*)(new[i]+j), res);
  }

  for (i = 1; i < rows + 1; i++){
    for (j = 1; j < cols + 1; j++){
      printf("%d",  new[i][j]);
    }
    printf("\n");
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
      if(posix_memalign((void*)&old[i], 16, ((cols + 2)*sizeof(char))*2) != 0){
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
    for (j = 0; j < (int)strlen(s) - 1; j++)
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
  for (i = 1; i < rows + 1; i++){
    for (j = 1; j < cols + 1; j++){
      printf("%c",  (old[i][j] == 0) ? '.' : 'O');
    }
    printf("\n");
  }
  // for(a = 0; a < steps; a++){
    mejorado(old, new, rows, cols);
    aux = new;
    new = old;
    old = aux;
    //}
  for (i = 1; i < rows + 1; i++){
    for (j = 1; j < cols + 1; j++){
      printf("%c",  (old[i][j] == 0) ? '.' : 'O');
    }
    printf("\n");
  }
  return 0;
}
