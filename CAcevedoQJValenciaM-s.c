/* 
Compilacion en LINUX:
gcc quicksortseq.c -o qss

Instrucciones ejecución
 
./qss -O < datafile.txt 

  O en {S,V} (Silencio o Verboso)  
  < datafile.txt (Nombre del archivo que contiene el arreglo a ordenar)

Ejemplo:
  
  ./qss -V < Datos500000.txt
   
  ch 777 experimento1random.sh
      
*/

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/types.h>

int *ReadData(int n);
void printData(int *data, int n);
void iterativeQuicksort(int *data, int left, int right);
int partition(int *data, int left, int right);
void swap(int *a, int *b);
void Usage(char *message);

int n,mode;
    
int main(int argc, char **argv) {
	
	int *data;

    if (argc == 1){  
          
		if (strcmp(argv[1],"-S") == 0)
		mode = 0;		
		if (strcmp(argv[1],"-V") == 0)
        mode = 1;   
            
			scanf("%d",&n);
			data=ReadData(n);
      
            
			if (mode == 1){	
				printf("\nArreglo Desordenado \n");
				printData(data,n);     
			}
			
			iterativeQuicksort(data, 0, n - 1);
			
			if (mode == 1){
				printf("\n\nArreglo Ordenado \n");
				printData(data,n);     
			}
			
			free(data); 
      }

	else
	Usage(argv[0]);  
	
	return 0;
}

int *ReadData(int n){
	
	int i;
	int *arreglo;
	arreglo= calloc(n,sizeof(int *));
  
		for (i = 0; i < n; ++i)
		scanf("%d",&arreglo[i]);
     
	return arreglo;
 
}

void printData(int *data, int n){

	int i;
		for (i = 0; i < n; i++){
			if (i % 5 != 0)
			printf("\t");
			else
			printf("\n");
    
		printf("%d",data[i]);
	}
	printf("\n");

}

void iterativeQuicksort(int *data, int left, int right){
   
    int *stack;    
    stack=calloc(right - left + 1,sizeof(int*));
    int top = -1;

    stack[ ++top ] = left;
    stack[ ++top ] = right;

    float E_cpu;
    long E_wall;
    time_t  ts, te; /* time_t is defined on <time.h> and <sys/types.h> as long */
    clock_t cs, ce; /* clock_t is defined on <time.h> and <sys/types.h> as int */    

    ts = time(NULL);
    cs = clock();
 
     while ( top >= 0 ){

        right = stack[ top-- ];
        left = stack[ top-- ];

        int p = partition( data, left, right );

        if ( p-1 > left ){
            stack[ ++top ] = left;
            stack[ ++top ] = p - 1;
        }

        if ( p+1 < right ){
            stack[ ++top ] = p + 1;
            stack[ ++top ] = right;
        }
        
	}
		ce = clock();
		te = time(NULL);
		E_wall = (long) (te - ts);
		E_cpu = (float)(ce - cs) / CLOCKS_PER_SEC;
		
		if (mode == 0)
		printf("%d %f,%ld\n",n,E_cpu,E_wall);
		
		free(stack);  
       
}

int partition(int *data, int left, int right){

  int pivot = (int)data[right];
  int i = left - 1;
  int j = right;
  while(1) {
    do {
      ++i;
    } while((int)data[i] < pivot);
    do {
      --j;
    } while((int)data[j] > pivot);
    
    if (i < j)
      swap(data + i,data +j);
    else {
      swap(data +i,data + right);
      return i;
    }
  }
}

void swap(int *a, int *b){
  int t = *a;
  *a = *b;
  *b = t;
}

void Usage(char *message){
  
   printf("\nUsage: %s -O Y N< datafile",message);
   printf("\n\nO in {S,V} ,Y in {O,1}\n\n,N in {T,n}\n\n");
}   
