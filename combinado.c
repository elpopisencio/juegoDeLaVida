
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <emmintrin.h>
#include <omp.h>

void mejorado(char** old, char** new, int rows, int cols){
  int i, j;
  __m128i sumacolant, sumacol, sumacolsig,res, resaux, ressig, resant, f1, f2, f2aux, f3, aux;
  //Copia bordes
  for (j = 0; j < cols; j = j + 16){

    f1 = _mm_load_si128((__m128i*)(old[rows]+j));
    _mm_store_si128((__m128i*)(old[0]+j), f1);

    f1 = _mm_load_si128((__m128i*)(old[1]+j));
    _mm_store_si128((__m128i*)(old[rows + 1]+j), f1);


  }
  for (i = 1; i < rows + 1; i++){
    old[i][0] = old[i][cols];
    old[i][cols + 1] = old[i][1];
  }
  old[0][0] = old[rows][cols];
  old[0][cols + 1] = old[rows][1];
  old[rows + 1][0] = old[1][cols];
  old[rows + 1][cols + 1] = old[1][1];

  //#pragma omp parallel for private (sumacolant, sumacol, sumacolsig,res, resaux, ressig, resant, f1, f2, f2aux, f3, aux, j) schedule (static)
    
   for (i = 1; i < rows + 1; i++){
  
  //primer vector
    f1 = _mm_load_si128((__m128i*)(old[i-1]+0));
    f2 = _mm_load_si128((__m128i*)(old[i]+0));
    f3 = _mm_load_si128((__m128i*)(old[i+1]+0));
    aux = _mm_add_epi8 (f1, f2);
    sumacol = _mm_add_epi8 (aux, f3);
    //siguiente
    f1 = _mm_load_si128((__m128i*)(old[i-1]+16));
    f2aux = _mm_load_si128((__m128i*)(old[i]+16));
    f3 = _mm_load_si128((__m128i*)(old[i+1]+16));
    aux = _mm_add_epi8 (f1, f2aux);
    sumacolsig = _mm_add_epi8 (aux, f3);
    
    aux =  _mm_srli_si128 (sumacol, 1);
    resaux = _mm_add_epi8 (sumacol, aux);
    aux = _mm_slli_si128 (sumacolsig, 15);
    resaux = _mm_add_epi8 (resaux, aux);
    aux =  _mm_slli_si128 (sumacol, 1);
    resaux = _mm_add_epi8 (resaux, aux);
    
    res = _mm_sub_epi8 (resaux, f2);

    f1 = res;
      f3 = _mm_set1_epi8(3);
      resaux = _mm_cmpeq_epi8(f1, f3);
      f3 = _mm_set1_epi8(2);
      res = _mm_cmpeq_epi8(f1, f3);
      f3 = _mm_set1_epi8(1);
      ressig = _mm_cmpeq_epi8(f3, f2);
      res = _mm_and_si128 (res, ressig);
      res = _mm_add_epi8 (res, resaux);
      f3 = _mm_set1_epi8(1);
      res = _mm_and_si128 (res, f3);
      _mm_store_si128((__m128i*)(new[i]+0), res);



    
    for (j = 16; j < cols - 16; j = j + 16){
      //anterior
      sumacolant = sumacol;
      //primer vector
      sumacol = sumacolsig;
      f2 = f2aux;
      //siguiente
      f1 = _mm_load_si128((__m128i*)(old[i-1]+j+16));
      f2aux = _mm_load_si128((__m128i*)(old[i]+j+16));
      f3 = _mm_load_si128((__m128i*)(old[i+1]+j+16));
      aux = _mm_add_epi8 (f1, f2aux);
      sumacolsig = _mm_add_epi8 (aux, f3);
   
      aux =  _mm_srli_si128 (sumacol, 1);
      resaux = _mm_add_epi8 (sumacol, aux);
      aux = _mm_slli_si128 (sumacolsig, 15);
      resaux = _mm_add_epi8 (resaux, aux);
      aux =  _mm_slli_si128 (sumacol, 1);
      resaux = _mm_add_epi8 (resaux, aux);
      aux =  _mm_srli_si128 (sumacolant, 15);
      resaux = _mm_add_epi8 (resaux, aux);

      
      res = _mm_sub_epi8 (resaux, f2);

      f1 = res;
      f3 = _mm_set1_epi8(3);
      resaux = _mm_cmpeq_epi8(f1, f3);
      f3 = _mm_set1_epi8(2);
      res = _mm_cmpeq_epi8(f1, f3);
      f3 = _mm_set1_epi8(1);
      ressig = _mm_cmpeq_epi8(f3, f2);
      res = _mm_and_si128 (res, ressig);
      res = _mm_add_epi8 (res, resaux);
      f3 = _mm_set1_epi8(1);
      res = _mm_and_si128 (res, f3);
      _mm_store_si128((__m128i*)(new[i]+j), res);


    }
      //anterior
      sumacolant = sumacol;
      //primer vector
      sumacol = sumacolsig;
      f2 = f2aux;
   
      aux =  _mm_srli_si128 (sumacol, 1);
      resaux = _mm_add_epi8 (sumacol, aux);
      aux =  _mm_slli_si128 (sumacol, 1);
      resaux = _mm_add_epi8 (resaux, aux);
      aux =  _mm_srli_si128 (sumacolant, 15);
      resaux = _mm_add_epi8 (resaux, aux);
      res = _mm_sub_epi8 (resaux, f2);

      f1 = res;
      f3 = _mm_set1_epi8(3);
      resaux = _mm_cmpeq_epi8(f1, f3);
      f3 = _mm_set1_epi8(2);
      res = _mm_cmpeq_epi8(f1, f3);
      f3 = _mm_set1_epi8(1);
      ressig = _mm_cmpeq_epi8(f3, f2);
      res = _mm_and_si128 (res, ressig);
      res = _mm_add_epi8 (res, resaux);
      f3 = _mm_set1_epi8(1);
      res = _mm_and_si128 (res, f3);
      _mm_store_si128((__m128i*)(new[i]+j), res);  
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
 
 for(a = 0; a < steps; a++){

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <emmintrin.h>
#include <omp.h>

void mejorado(char** old, char** new, int rows, int cols){
  int i, j;
  __m128i sumacolant, sumacol, sumacolsig,res, resaux, ressig, resant, f1, f2, f2aux, f3, aux;
  //Copia bordes
  for (j = 0; j < cols; j = j + 16){

    f1 = _mm_load_si128((__m128i*)(old[rows]+j));
    _mm_store_si128((__m128i*)(old[0]+j), f1);

    f1 = _mm_load_si128((__m128i*)(old[1]+j));
    _mm_store_si128((__m128i*)(old[rows + 1]+j), f1);


  }
  for (i = 1; i < rows + 1; i++){
    old[i][0] = old[i][cols];
    old[i][cols + 1] = old[i][1];
  }
  old[0][0] = old[rows][cols];
  old[0][cols + 1] = old[rows][1];
  old[rows + 1][0] = old[1][cols];
  old[rows + 1][cols + 1] = old[1][1];

  //#pragma omp parallel for private (sumacolant, sumacol, sumacolsig,res, resaux, ressig, resant, f1, f2, f2aux, f3, aux, j) schedule (static)
    
   for (i = 1; i < rows + 1; i++){
  
  //primer vector
    f1 = _mm_load_si128((__m128i*)(old[i-1]+0));
    f2 = _mm_load_si128((__m128i*)(old[i]+0));
    f3 = _mm_load_si128((__m128i*)(old[i+1]+0));
    aux = _mm_add_epi8 (f1, f2);
    sumacol = _mm_add_epi8 (aux, f3);
    //siguiente
    f1 = _mm_load_si128((__m128i*)(old[i-1]+16));
    f2aux = _mm_load_si128((__m128i*)(old[i]+16));
    f3 = _mm_load_si128((__m128i*)(old[i+1]+16));
    aux = _mm_add_epi8 (f1, f2aux);
    sumacolsig = _mm_add_epi8 (aux, f3);
    
    aux =  _mm_srli_si128 (sumacol, 1);
    resaux = _mm_add_epi8 (sumacol, aux);
    aux = _mm_slli_si128 (sumacolsig, 15);
    resaux = _mm_add_epi8 (resaux, aux);
    aux =  _mm_slli_si128 (sumacol, 1);
    resaux = _mm_add_epi8 (resaux, aux);
    
    res = _mm_sub_epi8 (resaux, f2);

    f1 = res;
      f3 = _mm_set1_epi8(3);
      resaux = _mm_cmpeq_epi8(f1, f3);
      f3 = _mm_set1_epi8(2);
      res = _mm_cmpeq_epi8(f1, f3);
      f3 = _mm_set1_epi8(1);
      ressig = _mm_cmpeq_epi8(f3, f2);
      res = _mm_and_si128 (res, ressig);
      res = _mm_add_epi8 (res, resaux);
      f3 = _mm_set1_epi8(1);
      res = _mm_and_si128 (res, f3);
      _mm_store_si128((__m128i*)(new[i]+0), res);



    
    for (j = 16; j < cols - 16; j = j + 16){
      //anterior
      sumacolant = sumacol;
      //primer vector
      sumacol = sumacolsig;
      f2 = f2aux;
      //siguiente
      f1 = _mm_load_si128((__m128i*)(old[i-1]+j+16));
      f2aux = _mm_load_si128((__m128i*)(old[i]+j+16));
      f3 = _mm_load_si128((__m128i*)(old[i+1]+j+16));
      aux = _mm_add_epi8 (f1, f2aux);
      sumacolsig = _mm_add_epi8 (aux, f3);
   
      aux =  _mm_srli_si128 (sumacol, 1);
      resaux = _mm_add_epi8 (sumacol, aux);
      aux = _mm_slli_si128 (sumacolsig, 15);
      resaux = _mm_add_epi8 (resaux, aux);
      aux =  _mm_slli_si128 (sumacol, 1);
      resaux = _mm_add_epi8 (resaux, aux);
      aux =  _mm_srli_si128 (sumacolant, 15);
      resaux = _mm_add_epi8 (resaux, aux);

      
      res = _mm_sub_epi8 (resaux, f2);

      f1 = res;
      f3 = _mm_set1_epi8(3);
      resaux = _mm_cmpeq_epi8(f1, f3);
      f3 = _mm_set1_epi8(2);
      res = _mm_cmpeq_epi8(f1, f3);
      f3 = _mm_set1_epi8(1);
      ressig = _mm_cmpeq_epi8(f3, f2);
      res = _mm_and_si128 (res, ressig);
      res = _mm_add_epi8 (res, resaux);
      f3 = _mm_set1_epi8(1);
      res = _mm_and_si128 (res, f3);
      _mm_store_si128((__m128i*)(new[i]+j), res);


    }
      //anterior
      sumacolant = sumacol;
      //primer vector
      sumacol = sumacolsig;
      f2 = f2aux;
   
      aux =  _mm_srli_si128 (sumacol, 1);
      resaux = _mm_add_epi8 (sumacol, aux);
      aux =  _mm_slli_si128 (sumacol, 1);
      resaux = _mm_add_epi8 (resaux, aux);
      aux =  _mm_srli_si128 (sumacolant, 15);
      resaux = _mm_add_epi8 (resaux, aux);
      res = _mm_sub_epi8 (resaux, f2);

      f1 = res;
      f3 = _mm_set1_epi8(3);
      resaux = _mm_cmpeq_epi8(f1, f3);
      f3 = _mm_set1_epi8(2);
      res = _mm_cmpeq_epi8(f1, f3);
      f3 = _mm_set1_epi8(1);
      ressig = _mm_cmpeq_epi8(f3, f2);
      res = _mm_and_si128 (res, ressig);
      res = _mm_add_epi8 (res, resaux);
      f3 = _mm_set1_epi8(1);
      res = _mm_and_si128 (res, f3);
      _mm_store_si128((__m128i*)(new[i]+j), res);  
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
 
 for(a = 0; a < steps; a++){

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <emmintrin.h>
#include <omp.h>

void mejorado(char** old, char** new, int rows, int cols){
  int i, j;
  __m128i sumacolant, sumacol, sumacolsig,res, resaux, ressig, resant, f1, f2, f2aux, f3, aux;
  //Copia bordes
  for (j = 0; j < cols; j = j + 16){

    f1 = _mm_load_si128((__m128i*)(old[rows]+j));
    _mm_store_si128((__m128i*)(old[0]+j), f1);

    f1 = _mm_load_si128((__m128i*)(old[1]+j));
    _mm_store_si128((__m128i*)(old[rows + 1]+j), f1);


  }
  for (i = 1; i < rows + 1; i++){
    old[i][0] = old[i][cols];
    old[i][cols + 1] = old[i][1];
  }
  old[0][0] = old[rows][cols];
  old[0][cols + 1] = old[rows][1];
  old[rows + 1][0] = old[1][cols];
  old[rows + 1][cols + 1] = old[1][1];

  //#pragma omp parallel for private (sumacolant, sumacol, sumacolsig,res, resaux, ressig, resant, f1, f2, f2aux, f3, aux, j) schedule (static)
    
   for (i = 1; i < rows + 1; i++){
  
  //primer vector
    f1 = _mm_load_si128((__m128i*)(old[i-1]+0));
    f2 = _mm_load_si128((__m128i*)(old[i]+0));
    f3 = _mm_load_si128((__m128i*)(old[i+1]+0));
    aux = _mm_add_epi8 (f1, f2);
    sumacol = _mm_add_epi8 (aux, f3);
    //siguiente
    f1 = _mm_load_si128((__m128i*)(old[i-1]+16));
    f2aux = _mm_load_si128((__m128i*)(old[i]+16));
    f3 = _mm_load_si128((__m128i*)(old[i+1]+16));
    aux = _mm_add_epi8 (f1, f2aux);
    sumacolsig = _mm_add_epi8 (aux, f3);
    
    aux =  _mm_srli_si128 (sumacol, 1);
    resaux = _mm_add_epi8 (sumacol, aux);
    aux = _mm_slli_si128 (sumacolsig, 15);
    resaux = _mm_add_epi8 (resaux, aux);
    aux =  _mm_slli_si128 (sumacol, 1);
    resaux = _mm_add_epi8 (resaux, aux);
    
    res = _mm_sub_epi8 (resaux, f2);

    f1 = res;
      f3 = _mm_set1_epi8(3);
      resaux = _mm_cmpeq_epi8(f1, f3);
      f3 = _mm_set1_epi8(2);
      res = _mm_cmpeq_epi8(f1, f3);
      f3 = _mm_set1_epi8(1);
      ressig = _mm_cmpeq_epi8(f3, f2);
      res = _mm_and_si128 (res, ressig);
      res = _mm_add_epi8 (res, resaux);
      f3 = _mm_set1_epi8(1);
      res = _mm_and_si128 (res, f3);
      _mm_store_si128((__m128i*)(new[i]+0), res);



    
    for (j = 16; j < cols - 16; j = j + 16){
      //anterior
      sumacolant = sumacol;
      //primer vector
      sumacol = sumacolsig;
      f2 = f2aux;
      //siguiente
      f1 = _mm_load_si128((__m128i*)(old[i-1]+j+16));
      f2aux = _mm_load_si128((__m128i*)(old[i]+j+16));
      f3 = _mm_load_si128((__m128i*)(old[i+1]+j+16));
      aux = _mm_add_epi8 (f1, f2aux);
      sumacolsig = _mm_add_epi8 (aux, f3);
   
      aux =  _mm_srli_si128 (sumacol, 1);
      resaux = _mm_add_epi8 (sumacol, aux);
      aux = _mm_slli_si128 (sumacolsig, 15);
      resaux = _mm_add_epi8 (resaux, aux);
      aux =  _mm_slli_si128 (sumacol, 1);
      resaux = _mm_add_epi8 (resaux, aux);
      aux =  _mm_srli_si128 (sumacolant, 15);
      resaux = _mm_add_epi8 (resaux, aux);

      
      res = _mm_sub_epi8 (resaux, f2);

      f1 = res;
      f3 = _mm_set1_epi8(3);
      resaux = _mm_cmpeq_epi8(f1, f3);
      f3 = _mm_set1_epi8(2);
      res = _mm_cmpeq_epi8(f1, f3);
      f3 = _mm_set1_epi8(1);
      ressig = _mm_cmpeq_epi8(f3, f2);
      res = _mm_and_si128 (res, ressig);
      res = _mm_add_epi8 (res, resaux);
      f3 = _mm_set1_epi8(1);
      res = _mm_and_si128 (res, f3);
      _mm_store_si128((__m128i*)(new[i]+j), res);


    }
      //anterior
      sumacolant = sumacol;
      //primer vector
      sumacol = sumacolsig;
      f2 = f2aux;
   
      aux =  _mm_srli_si128 (sumacol, 1);
      resaux = _mm_add_epi8 (sumacol, aux);
      aux =  _mm_slli_si128 (sumacol, 1);
      resaux = _mm_add_epi8 (resaux, aux);
      aux =  _mm_srli_si128 (sumacolant, 15);
      resaux = _mm_add_epi8 (resaux, aux);
      res = _mm_sub_epi8 (resaux, f2);

      f1 = res;
      f3 = _mm_set1_epi8(3);
      resaux = _mm_cmpeq_epi8(f1, f3);
      f3 = _mm_set1_epi8(2);
      res = _mm_cmpeq_epi8(f1, f3);
      f3 = _mm_set1_epi8(1);
      ressig = _mm_cmpeq_epi8(f3, f2);
      res = _mm_and_si128 (res, ressig);
      res = _mm_add_epi8 (res, resaux);
      f3 = _mm_set1_epi8(1);
      res = _mm_and_si128 (res, f3);
      _mm_store_si128((__m128i*)(new[i]+j), res);  
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
 
 for(a = 0; a < steps; a++){

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <emmintrin.h>
#include <omp.h>

void mejorado(char** old, char** new, int rows, int cols){
  int i, j;
  __m128i sumacolant, sumacol, sumacolsig,res, resaux, ressig, resant, f1, f2, f2aux, f3, aux;
  //Copia bordes
  for (j = 0; j < cols; j = j + 16){

    f1 = _mm_load_si128((__m128i*)(old[rows]+j));
    _mm_store_si128((__m128i*)(old[0]+j), f1);

    f1 = _mm_load_si128((__m128i*)(old[1]+j));
    _mm_store_si128((__m128i*)(old[rows + 1]+j), f1);


  }
  for (i = 1; i < rows + 1; i++){
    old[i][0] = old[i][cols];
    old[i][cols + 1] = old[i][1];
  }
  old[0][0] = old[rows][cols];
  old[0][cols + 1] = old[rows][1];
  old[rows + 1][0] = old[1][cols];
  old[rows + 1][cols + 1] = old[1][1];

  //#pragma omp parallel for private (sumacolant, sumacol, sumacolsig,res, resaux, ressig, resant, f1, f2, f2aux, f3, aux, j) schedule (static)
    
   for (i = 1; i < rows + 1; i++){
  
  //primer vector
    f1 = _mm_load_si128((__m128i*)(old[i-1]+0));
    f2 = _mm_load_si128((__m128i*)(old[i]+0));
    f3 = _mm_load_si128((__m128i*)(old[i+1]+0));
    aux = _mm_add_epi8 (f1, f2);
    sumacol = _mm_add_epi8 (aux, f3);
    //siguiente
    f1 = _mm_load_si128((__m128i*)(old[i-1]+16));
    f2aux = _mm_load_si128((__m128i*)(old[i]+16));
    f3 = _mm_load_si128((__m128i*)(old[i+1]+16));
    aux = _mm_add_epi8 (f1, f2aux);
    sumacolsig = _mm_add_epi8 (aux, f3);
    
    aux =  _mm_srli_si128 (sumacol, 1);
    resaux = _mm_add_epi8 (sumacol, aux);
    aux = _mm_slli_si128 (sumacolsig, 15);
    resaux = _mm_add_epi8 (resaux, aux);
    aux =  _mm_slli_si128 (sumacol, 1);
    resaux = _mm_add_epi8 (resaux, aux);
    
    res = _mm_sub_epi8 (resaux, f2);

    f1 = res;
      f3 = _mm_set1_epi8(3);
      resaux = _mm_cmpeq_epi8(f1, f3);
      f3 = _mm_set1_epi8(2);
      res = _mm_cmpeq_epi8(f1, f3);
      f3 = _mm_set1_epi8(1);
      ressig = _mm_cmpeq_epi8(f3, f2);
      res = _mm_and_si128 (res, ressig);
      res = _mm_add_epi8 (res, resaux);
      f3 = _mm_set1_epi8(1);
      res = _mm_and_si128 (res, f3);
      _mm_store_si128((__m128i*)(new[i]+0), res);



    
    for (j = 16; j < cols - 16; j = j + 16){
      //anterior
      sumacolant = sumacol;
      //primer vector
      sumacol = sumacolsig;
      f2 = f2aux;
      //siguiente
      f1 = _mm_load_si128((__m128i*)(old[i-1]+j+16));
      f2aux = _mm_load_si128((__m128i*)(old[i]+j+16));
      f3 = _mm_load_si128((__m128i*)(old[i+1]+j+16));
      aux = _mm_add_epi8 (f1, f2aux);
      sumacolsig = _mm_add_epi8 (aux, f3);
   
      aux =  _mm_srli_si128 (sumacol, 1);
      resaux = _mm_add_epi8 (sumacol, aux);
      aux = _mm_slli_si128 (sumacolsig, 15);
      resaux = _mm_add_epi8 (resaux, aux);
      aux =  _mm_slli_si128 (sumacol, 1);
      resaux = _mm_add_epi8 (resaux, aux);
      aux =  _mm_srli_si128 (sumacolant, 15);
      resaux = _mm_add_epi8 (resaux, aux);

      
      res = _mm_sub_epi8 (resaux, f2);

      f1 = res;
      f3 = _mm_set1_epi8(3);
      resaux = _mm_cmpeq_epi8(f1, f3);
      f3 = _mm_set1_epi8(2);
      res = _mm_cmpeq_epi8(f1, f3);
      f3 = _mm_set1_epi8(1);
      ressig = _mm_cmpeq_epi8(f3, f2);
      res = _mm_and_si128 (res, ressig);
      res = _mm_add_epi8 (res, resaux);
      f3 = _mm_set1_epi8(1);
      res = _mm_and_si128 (res, f3);
      _mm_store_si128((__m128i*)(new[i]+j), res);


    }
      //anterior
      sumacolant = sumacol;
      //primer vector
      sumacol = sumacolsig;
      f2 = f2aux;
   
      aux =  _mm_srli_si128 (sumacol, 1);
      resaux = _mm_add_epi8 (sumacol, aux);
      aux =  _mm_slli_si128 (sumacol, 1);
      resaux = _mm_add_epi8 (resaux, aux);
      aux =  _mm_srli_si128 (sumacolant, 15);
      resaux = _mm_add_epi8 (resaux, aux);
      res = _mm_sub_epi8 (resaux, f2);

      f1 = res;
      f3 = _mm_set1_epi8(3);
      resaux = _mm_cmpeq_epi8(f1, f3);
      f3 = _mm_set1_epi8(2);
      res = _mm_cmpeq_epi8(f1, f3);
      f3 = _mm_set1_epi8(1);
      ressig = _mm_cmpeq_epi8(f3, f2);
      res = _mm_and_si128 (res, ressig);
      res = _mm_add_epi8 (res, resaux);
      f3 = _mm_set1_epi8(1);
      res = _mm_and_si128 (res, f3);
      _mm_store_si128((__m128i*)(new[i]+j), res);  
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
 
 for(a = 0; a < steps; a++){

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <emmintrin.h>
#include <omp.h>

void mejorado(char** old, char** new, int rows, int cols){
  int i, j;
  __m128i sumacolant, sumacol, sumacolsig,res, resaux, ressig, resant, f1, f2, f2aux, f3, aux;
  //Copia bordes
  for (j = 0; j < cols; j = j + 16){

    f1 = _mm_load_si128((__m128i*)(old[rows]+j));
    _mm_store_si128((__m128i*)(old[0]+j), f1);

    f1 = _mm_load_si128((__m128i*)(old[1]+j));
    _mm_store_si128((__m128i*)(old[rows + 1]+j), f1);


  }
  for (i = 1; i < rows + 1; i++){
    old[i][0] = old[i][cols];
    old[i][cols + 1] = old[i][1];
  }
  old[0][0] = old[rows][cols];
  old[0][cols + 1] = old[rows][1];
  old[rows + 1][0] = old[1][cols];
  old[rows + 1][cols + 1] = old[1][1];

  //#pragma omp parallel for private (sumacolant, sumacol, sumacolsig,res, resaux, ressig, resant, f1, f2, f2aux, f3, aux, j) schedule (static)
    
   for (i = 1; i < rows + 1; i++){
  
  //primer vector
    f1 = _mm_load_si128((__m128i*)(old[i-1]+0));
    f2 = _mm_load_si128((__m128i*)(old[i]+0));
    f3 = _mm_load_si128((__m128i*)(old[i+1]+0));
    aux = _mm_add_epi8 (f1, f2);
    sumacol = _mm_add_epi8 (aux, f3);
    //siguiente
    f1 = _mm_load_si128((__m128i*)(old[i-1]+16));
    f2aux = _mm_load_si128((__m128i*)(old[i]+16));
    f3 = _mm_load_si128((__m128i*)(old[i+1]+16));
    aux = _mm_add_epi8 (f1, f2aux);
    sumacolsig = _mm_add_epi8 (aux, f3);
    
    aux =  _mm_srli_si128 (sumacol, 1);
    resaux = _mm_add_epi8 (sumacol, aux);
    aux = _mm_slli_si128 (sumacolsig, 15);
    resaux = _mm_add_epi8 (resaux, aux);
    aux =  _mm_slli_si128 (sumacol, 1);
    resaux = _mm_add_epi8 (resaux, aux);
    
    res = _mm_sub_epi8 (resaux, f2);

    f1 = res;
      f3 = _mm_set1_epi8(3);
      resaux = _mm_cmpeq_epi8(f1, f3);
      f3 = _mm_set1_epi8(2);
      res = _mm_cmpeq_epi8(f1, f3);
      f3 = _mm_set1_epi8(1);
      ressig = _mm_cmpeq_epi8(f3, f2);
      res = _mm_and_si128 (res, ressig);
      res = _mm_add_epi8 (res, resaux);
      f3 = _mm_set1_epi8(1);
      res = _mm_and_si128 (res, f3);
      _mm_store_si128((__m128i*)(new[i]+0), res);



    
    for (j = 16; j < cols - 16; j = j + 16){
      //anterior
      sumacolant = sumacol;
      //primer vector
      sumacol = sumacolsig;
      f2 = f2aux;
      //siguiente
      f1 = _mm_load_si128((__m128i*)(old[i-1]+j+16));
      f2aux = _mm_load_si128((__m128i*)(old[i]+j+16));
      f3 = _mm_load_si128((__m128i*)(old[i+1]+j+16));
      aux = _mm_add_epi8 (f1, f2aux);
      sumacolsig = _mm_add_epi8 (aux, f3);
   
      aux =  _mm_srli_si128 (sumacol, 1);
      resaux = _mm_add_epi8 (sumacol, aux);
      aux = _mm_slli_si128 (sumacolsig, 15);
      resaux = _mm_add_epi8 (resaux, aux);
      aux =  _mm_slli_si128 (sumacol, 1);
      resaux = _mm_add_epi8 (resaux, aux);
      aux =  _mm_srli_si128 (sumacolant, 15);
      resaux = _mm_add_epi8 (resaux, aux);

      
      res = _mm_sub_epi8 (resaux, f2);

      f1 = res;
      f3 = _mm_set1_epi8(3);
      resaux = _mm_cmpeq_epi8(f1, f3);
      f3 = _mm_set1_epi8(2);
      res = _mm_cmpeq_epi8(f1, f3);
      f3 = _mm_set1_epi8(1);
      ressig = _mm_cmpeq_epi8(f3, f2);
      res = _mm_and_si128 (res, ressig);
      res = _mm_add_epi8 (res, resaux);
      f3 = _mm_set1_epi8(1);
      res = _mm_and_si128 (res, f3);
      _mm_store_si128((__m128i*)(new[i]+j), res);


    }
      //anterior
      sumacolant = sumacol;
      //primer vector
      sumacol = sumacolsig;
      f2 = f2aux;
   
      aux =  _mm_srli_si128 (sumacol, 1);
      resaux = _mm_add_epi8 (sumacol, aux);
      aux =  _mm_slli_si128 (sumacol, 1);
      resaux = _mm_add_epi8 (resaux, aux);
      aux =  _mm_srli_si128 (sumacolant, 15);
      resaux = _mm_add_epi8 (resaux, aux);
      res = _mm_sub_epi8 (resaux, f2);

      f1 = res;
      f3 = _mm_set1_epi8(3);
      resaux = _mm_cmpeq_epi8(f1, f3);
      f3 = _mm_set1_epi8(2);
      res = _mm_cmpeq_epi8(f1, f3);
      f3 = _mm_set1_epi8(1);
      ressig = _mm_cmpeq_epi8(f3, f2);
      res = _mm_and_si128 (res, ressig);
      res = _mm_add_epi8 (res, resaux);
      f3 = _mm_set1_epi8(1);
      res = _mm_and_si128 (res, f3);
      _mm_store_si128((__m128i*)(new[i]+j), res);  
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
 
 for(a = 0; a < steps; a++){

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <emmintrin.h>
#include <omp.h>

void mejorado(char** old, char** new, int rows, int cols){
  int i, j;
  __m128i sumacolant, sumacol, sumacolsig,res, resaux, ressig, resant, f1, f2, f2aux, f3, aux;
  //Copia bordes
  for (j = 0; j < cols; j = j + 16){

    f1 = _mm_load_si128((__m128i*)(old[rows]+j));
    _mm_store_si128((__m128i*)(old[0]+j), f1);

    f1 = _mm_load_si128((__m128i*)(old[1]+j));
    _mm_store_si128((__m128i*)(old[rows + 1]+j), f1);


  }
  for (i = 1; i < rows + 1; i++){
    old[i][0] = old[i][cols];
    old[i][cols + 1] = old[i][1];
  }
  old[0][0] = old[rows][cols];
  old[0][cols + 1] = old[rows][1];
  old[rows + 1][0] = old[1][cols];
  old[rows + 1][cols + 1] = old[1][1];

  //#pragma omp parallel for private (sumacolant, sumacol, sumacolsig,res, resaux, ressig, resant, f1, f2, f2aux, f3, aux, j) schedule (static)
    
   for (i = 1; i < rows + 1; i++){
  
  //primer vector
    f1 = _mm_load_si128((__m128i*)(old[i-1]+0));
    f2 = _mm_load_si128((__m128i*)(old[i]+0));
    f3 = _mm_load_si128((__m128i*)(old[i+1]+0));
    aux = _mm_add_epi8 (f1, f2);
    sumacol = _mm_add_epi8 (aux, f3);
    //siguiente
    f1 = _mm_load_si128((__m128i*)(old[i-1]+16));
    f2aux = _mm_load_si128((__m128i*)(old[i]+16));
    f3 = _mm_load_si128((__m128i*)(old[i+1]+16));
    aux = _mm_add_epi8 (f1, f2aux);
    sumacolsig = _mm_add_epi8 (aux, f3);
    
    aux =  _mm_srli_si128 (sumacol, 1);
    resaux = _mm_add_epi8 (sumacol, aux);
    aux = _mm_slli_si128 (sumacolsig, 15);
    resaux = _mm_add_epi8 (resaux, aux);
    aux =  _mm_slli_si128 (sumacol, 1);
    resaux = _mm_add_epi8 (resaux, aux);
    
    res = _mm_sub_epi8 (resaux, f2);

    f1 = res;
      f3 = _mm_set1_epi8(3);
      resaux = _mm_cmpeq_epi8(f1, f3);
      f3 = _mm_set1_epi8(2);
      res = _mm_cmpeq_epi8(f1, f3);
      f3 = _mm_set1_epi8(1);
      ressig = _mm_cmpeq_epi8(f3, f2);
      res = _mm_and_si128 (res, ressig);
      res = _mm_add_epi8 (res, resaux);
      f3 = _mm_set1_epi8(1);
      res = _mm_and_si128 (res, f3);
      _mm_store_si128((__m128i*)(new[i]+0), res);



    
    for (j = 16; j < cols - 16; j = j + 16){
      //anterior
      sumacolant = sumacol;
      //primer vector
      sumacol = sumacolsig;
      f2 = f2aux;
      //siguiente
      f1 = _mm_load_si128((__m128i*)(old[i-1]+j+16));
      f2aux = _mm_load_si128((__m128i*)(old[i]+j+16));
      f3 = _mm_load_si128((__m128i*)(old[i+1]+j+16));
      aux = _mm_add_epi8 (f1, f2aux);
      sumacolsig = _mm_add_epi8 (aux, f3);
   
      aux =  _mm_srli_si128 (sumacol, 1);
      resaux = _mm_add_epi8 (sumacol, aux);
      aux = _mm_slli_si128 (sumacolsig, 15);
      resaux = _mm_add_epi8 (resaux, aux);
      aux =  _mm_slli_si128 (sumacol, 1);
      resaux = _mm_add_epi8 (resaux, aux);
      aux =  _mm_srli_si128 (sumacolant, 15);
      resaux = _mm_add_epi8 (resaux, aux);

      
      res = _mm_sub_epi8 (resaux, f2);

      f1 = res;
      f3 = _mm_set1_epi8(3);
      resaux = _mm_cmpeq_epi8(f1, f3);
      f3 = _mm_set1_epi8(2);
      res = _mm_cmpeq_epi8(f1, f3);
      f3 = _mm_set1_epi8(1);
      ressig = _mm_cmpeq_epi8(f3, f2);
      res = _mm_and_si128 (res, ressig);
      res = _mm_add_epi8 (res, resaux);
      f3 = _mm_set1_epi8(1);
      res = _mm_and_si128 (res, f3);
      _mm_store_si128((__m128i*)(new[i]+j), res);


    }
      //anterior
      sumacolant = sumacol;
      //primer vector
      sumacol = sumacolsig;
      f2 = f2aux;
   
      aux =  _mm_srli_si128 (sumacol, 1);
      resaux = _mm_add_epi8 (sumacol, aux);
      aux =  _mm_slli_si128 (sumacol, 1);
      resaux = _mm_add_epi8 (resaux, aux);
      aux =  _mm_srli_si128 (sumacolant, 15);
      resaux = _mm_add_epi8 (resaux, aux);
      res = _mm_sub_epi8 (resaux, f2);

      f1 = res;
      f3 = _mm_set1_epi8(3);
      resaux = _mm_cmpeq_epi8(f1, f3);
      f3 = _mm_set1_epi8(2);
      res = _mm_cmpeq_epi8(f1, f3);
      f3 = _mm_set1_epi8(1);
      ressig = _mm_cmpeq_epi8(f3, f2);
      res = _mm_and_si128 (res, ressig);
      res = _mm_add_epi8 (res, resaux);
      f3 = _mm_set1_epi8(1);
      res = _mm_and_si128 (res, f3);
      _mm_store_si128((__m128i*)(new[i]+j), res);  
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
 
 for(a = 0; a < steps; a++){

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <emmintrin.h>
#include <omp.h>

void mejorado(char** old, char** new, int rows, int cols){
  int i, j;
  __m128i sumacolant, sumacol, sumacolsig,res, resaux, ressig, resant, f1, f2, f2aux, f3, aux;
  //Copia bordes
  for (j = 0; j < cols; j = j + 16){

    f1 = _mm_load_si128((__m128i*)(old[rows]+j));
    _mm_store_si128((__m128i*)(old[0]+j), f1);

    f1 = _mm_load_si128((__m128i*)(old[1]+j));
    _mm_store_si128((__m128i*)(old[rows + 1]+j), f1);


  }
  for (i = 1; i < rows + 1; i++){
    old[i][0] = old[i][cols];
    old[i][cols + 1] = old[i][1];
  }
  old[0][0] = old[rows][cols];
  old[0][cols + 1] = old[rows][1];
  old[rows + 1][0] = old[1][cols];
  old[rows + 1][cols + 1] = old[1][1];

  //#pragma omp parallel for private (sumacolant, sumacol, sumacolsig,res, resaux, ressig, resant, f1, f2, f2aux, f3, aux, j) schedule (static)
    
   for (i = 1; i < rows + 1; i++){
  
  //primer vector
    f1 = _mm_load_si128((__m128i*)(old[i-1]+0));
    f2 = _mm_load_si128((__m128i*)(old[i]+0));
    f3 = _mm_load_si128((__m128i*)(old[i+1]+0));
    aux = _mm_add_epi8 (f1, f2);
    sumacol = _mm_add_epi8 (aux, f3);
    //siguiente
    f1 = _mm_load_si128((__m128i*)(old[i-1]+16));
    f2aux = _mm_load_si128((__m128i*)(old[i]+16));
    f3 = _mm_load_si128((__m128i*)(old[i+1]+16));
    aux = _mm_add_epi8 (f1, f2aux);
    sumacolsig = _mm_add_epi8 (aux, f3);
    
    aux =  _mm_srli_si128 (sumacol, 1);
    resaux = _mm_add_epi8 (sumacol, aux);
    aux = _mm_slli_si128 (sumacolsig, 15);
    resaux = _mm_add_epi8 (resaux, aux);
    aux =  _mm_slli_si128 (sumacol, 1);
    resaux = _mm_add_epi8 (resaux, aux);
    
    res = _mm_sub_epi8 (resaux, f2);

    f1 = res;
      f3 = _mm_set1_epi8(3);
      resaux = _mm_cmpeq_epi8(f1, f3);
      f3 = _mm_set1_epi8(2);
      res = _mm_cmpeq_epi8(f1, f3);
      f3 = _mm_set1_epi8(1);
      ressig = _mm_cmpeq_epi8(f3, f2);
      res = _mm_and_si128 (res, ressig);
      res = _mm_add_epi8 (res, resaux);
      f3 = _mm_set1_epi8(1);
      res = _mm_and_si128 (res, f3);
      _mm_store_si128((__m128i*)(new[i]+0), res);



    
    for (j = 16; j < cols - 16; j = j + 16){
      //anterior
      sumacolant = sumacol;
      //primer vector
      sumacol = sumacolsig;
      f2 = f2aux;
      //siguiente
      f1 = _mm_load_si128((__m128i*)(old[i-1]+j+16));
      f2aux = _mm_load_si128((__m128i*)(old[i]+j+16));
      f3 = _mm_load_si128((__m128i*)(old[i+1]+j+16));
      aux = _mm_add_epi8 (f1, f2aux);
      sumacolsig = _mm_add_epi8 (aux, f3);
   
      aux =  _mm_srli_si128 (sumacol, 1);
      resaux = _mm_add_epi8 (sumacol, aux);
      aux = _mm_slli_si128 (sumacolsig, 15);
      resaux = _mm_add_epi8 (resaux, aux);
      aux =  _mm_slli_si128 (sumacol, 1);
      resaux = _mm_add_epi8 (resaux, aux);
      aux =  _mm_srli_si128 (sumacolant, 15);
      resaux = _mm_add_epi8 (resaux, aux);

      
      res = _mm_sub_epi8 (resaux, f2);

      f1 = res;
      f3 = _mm_set1_epi8(3);
      resaux = _mm_cmpeq_epi8(f1, f3);
      f3 = _mm_set1_epi8(2);
      res = _mm_cmpeq_epi8(f1, f3);
      f3 = _mm_set1_epi8(1);
      ressig = _mm_cmpeq_epi8(f3, f2);
      res = _mm_and_si128 (res, ressig);
      res = _mm_add_epi8 (res, resaux);
      f3 = _mm_set1_epi8(1);
      res = _mm_and_si128 (res, f3);
      _mm_store_si128((__m128i*)(new[i]+j), res);


    }
      //anterior
      sumacolant = sumacol;
      //primer vector
      sumacol = sumacolsig;
      f2 = f2aux;
   
      aux =  _mm_srli_si128 (sumacol, 1);
      resaux = _mm_add_epi8 (sumacol, aux);
      aux =  _mm_slli_si128 (sumacol, 1);
      resaux = _mm_add_epi8 (resaux, aux);
      aux =  _mm_srli_si128 (sumacolant, 15);
      resaux = _mm_add_epi8 (resaux, aux);
      res = _mm_sub_epi8 (resaux, f2);

      f1 = res;
      f3 = _mm_set1_epi8(3);
      resaux = _mm_cmpeq_epi8(f1, f3);
      f3 = _mm_set1_epi8(2);
      res = _mm_cmpeq_epi8(f1, f3);
      f3 = _mm_set1_epi8(1);
      ressig = _mm_cmpeq_epi8(f3, f2);
      res = _mm_and_si128 (res, ressig);
      res = _mm_add_epi8 (res, resaux);
      f3 = _mm_set1_epi8(1);
      res = _mm_and_si128 (res, f3);
      _mm_store_si128((__m128i*)(new[i]+j), res);  
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
   //mejor(old, viv1, viv2, rows, cols);
   mejorado(old, new, rows, cols);

  aux1 = new;
    new = old;
    old = aux1;
}
  for (i = 1; i < rows + 1; i++){
    for (j = 1; j < cols + 1; j++){
      printf("%c",  (old[i][j] == 0) ? '.' : 'O');
    }
    printf("\n");
  }
  
  free(old);
  
  return 0;
}
