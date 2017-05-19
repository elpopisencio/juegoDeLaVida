// Integrantes: Beroisa Jorge, Gutierrez Jeremias.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node {
  int val;
  struct node * next;
} node_t;

void addNode(node_t ** nodo, int i){
  
  node_t * nod = *nodo;

  while(nod->next != NULL && nod->val != 0){
    nod = nod->next;
  }
  if(nod->val == 0){
    nod->val = i;
  }else{
    nod->next = malloc(sizeof(struct node));
    nod = nod->next;
    nod->val = i;
    nod->next = NULL; 
  }
}

void printRow(node_t ** nodo, int cols){
  int i;
  node_t * nod = *nodo;
  for(i=1 ; i <= cols; i++){
    if(nod != NULL && nod->val != 0){
      if(nod->val == i){
	printf("O");
	nod = nod->next;
      }else{
	printf(".");
      }
    }else{
      printf(".");
    }
  }
}
void printRowComp(node_t * nodo){
  node_t * nod = nodo;
  while(nod != NULL && nod->val != 0){
    printf("%d - ", nod->val);
    nod = nod->next;
  }
}

int sumar(node_t * row, int a1, int a2, int a3){
  node_t * nod = row;
  int i = 0;
  if(a3 != 0){
    while(nod != NULL && nod->val != 0){
      if(nod->val == a1 || nod->val == a2 || nod->val == a3){
	i++;
      }
      nod = nod->next;
    }
  } else{
    while(nod != NULL && nod->val != 0){
      if(nod->val == a1 || nod->val == a2){
	i++;
      }
      nod = nod->next;
    }
  }
  return i;
}

int buscar(node_t * row, int a){
  node_t * nod = row;
  int i = 0;
  while(nod != NULL && nod->val != 0){
    if(nod->val == a){
      i = 1;
      nod = NULL;
    }else{
      nod = nod->next;
    }
  }
  return i;
}



void actualizar(node_t ** old, node_t ** new, int rows, int cols, int i, int j){
  int suma = 0;
  // Si esta en la primer fila:
  if (i == 1){
    // Si es el primer elemento de la primer fila:  
    if( j == 1){
      suma += sumar(old[rows], cols, 1, 2);
      suma += sumar(old[1], cols, 2, 0);
      suma += sumar(old[2], cols, 1, 2);
    }else{
      // si esta en el medio
      if(j< cols){
	suma += sumar(old[rows], j-1, j, j+1);
	suma += sumar(old[1], j-1, j+1, 0);
	suma += sumar(old[2], j-1, j, j+1);
      }else{
	// Si esta al final
	suma += sumar(old[rows], cols-1, cols, 1);
	suma += sumar(old[1], cols-1, 1, 0);
	suma += sumar(old[2], cols-1, cols, 1);
      }
    }
  }else{
    if(i < rows){
      // Recorro las filas intermedias:
      if(j == 1){
	// Verifico el primer elemento de la fila i:
	suma += sumar(old[i-1], cols, 1, 2);
	suma += sumar(old[i], cols, 2, 0);
	suma += sumar(old[i+1], cols, 1, 2);
      }else{
	//si esta en el medio
	if(j < cols){
	  suma += sumar(old[i-1], j-1, j, j+1);
	  suma += sumar(old[i], j-1, j+1, 0);
	  suma += sumar(old[i+1], j-1, j, j+1);
	}else{
	  // Si esta al final
	  suma += sumar(old[i-1], cols-1, cols, 1);
	  suma += sumar(old[i], cols-1, 1, 0);
	  suma += sumar(old[i+1], cols-1, cols, 1);
	}
      } 
    } else{
      // Recorro la ultima fila:
      if(j == 1){
	// Verifico el primer elemento de la fila i:
	suma += sumar(old[rows-1], cols, 1, 2);
 	suma += sumar(old[rows], cols, 2, 0);
	suma += sumar(old[1], cols, 1, 2);
      }else{
	//si esta en el medio
	if(j < cols){
	  suma += sumar(old[rows-1], j-1, j, j+1);
	  suma += sumar(old[rows], j-1, j+1, 0);
	  suma += sumar(old[1], j-1, j, j+1);
	}else{
	  // Si esta al final
	  suma += sumar(old[rows-1], cols-1, cols, 1);
	  suma += sumar(old[rows], cols-1, 1, 0);
	  suma += sumar(old[1], cols-1, cols, 1);
	}
      } 
    }  
  }
  if(suma == 3 || (suma == 2 && buscar(old[i], j) ==  1)){
    addNode(&new[i], j);
  }
}

void mejor(node_t ** old, node_t ** new, int rows, int cols){
  int i;    
  node_t * nod;
  for(i = 1; i <= rows; i++){
    new[i]->val = 0;
  }
  for(i = 1; i <= rows; i++){  
    nod = old[i];
    while(nod != NULL && nod->val != 0){
      actualizar(old, new, rows, cols, i-1, nod->val -1);
      actualizar(old, new, rows, cols, i-1, nod->val);
      actualizar(old, new, rows, cols, i-1, nod->val +1);

      actualizar(old, new, rows, cols, i, nod->val -1);
      actualizar(old, new, rows, cols, i, nod->val);
      actualizar(old, new, rows, cols, i, nod->val +1);

      actualizar(old, new, rows, cols, i+1, nod->val -1);
      actualizar(old, new, rows, cols, i+1, nod->val);
      actualizar(old, new, rows, cols, i+1, nod->val +1);

      nod = nod->next;
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
  node_t ** old;
  node_t ** new;
  node_t ** aux;
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
  old = malloc((rows + 2)*sizeof(node_t*));
  new = malloc((rows + 2)*sizeof(node_t*));
  
  if(old != NULL && new != NULL ){
    for (i=0;i<(rows + 2);i++){  	
      old[i] = malloc(sizeof(node_t));
      old[i]->val = 0;
      old[i]->next = NULL;
      new[i] = malloc(sizeof(node_t));
      new[i]->val = 0;
      new[i]->next = NULL;      

      if(old[i] == NULL && new[i] == NULL){
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
 
  i = 1;
  
  while (res != NULL) {
    
    for (j = 0; j < (int)strlen(s) - 1; j++){
      if(s[j] == '.'){
	//old[i][j+1] = 0;
      } else{
	addNode(&old[i], j+1);
      }
    }
    res = fgets(s, cols, f);
    i++;
  }
  
  fclose(f);
  free(s);
  
  /*************************************************
		 Resto del programa
 		 ...
		 ...
        
  **************************************************/

  int a;
    for(a = 0; a < steps; a++){
    
    // viejo(old, new, rows, cols);
                mejor(old, new, rows, cols);


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

    aux = new;
    new = old;
    old = aux;
 
    } 

    /*
    for (i = 1; i < rows + 1; i++){
    for (j = 1; j < cols + 1; j++){
    printf("%c",  (old[i][j] == 1) ? 'O' : '.');
    }
    printf("\n");
    }
  */

  for (i = 1; i < rows + 1; i++){
    //printf("Fila %d: ", i);
    printRow(&old[i], cols);
    printf("\n");
  }

  return 0;
}
