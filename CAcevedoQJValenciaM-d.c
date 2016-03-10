/*****************************************************************************
 * 
 * FILE: CAcevedoQJValenciaM-d.c 
 *
 * DESCRIPTION: Distribuye el arreglo de entrada a los numproc procesadores yrealiza 
		Quicksort iterativo por cada proceso (incluyendo el maestro). 
		Finalmentese se realiza  merge sort entre los procesos vecinos, 
		para obtener el resultado final. 
 * 
 * AUTHOR: Cristóbal Acevedo Jorge Valencia
 *
 * LAST REVISED: Santiago de Chile, 26/11/2014
 *
 *****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>


#define SILENT  0
#define VERBOSE 1
#define ABORT   -1
#define MASTER  0


#define TAG0    0

#define TAG1    1
#define TAG2    2

#define TAG3     3

#define TAG4     4


//Intercambio de elementos
void swap(int *a, int *b) {
  int t = *a;
  *a = *b;
  *b = t;
}



//Función que particiona el arreglo.
int partition(int *data, int left, int right) {

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


//Funcion que realiza la emulación, a través de una pila,
// de la naturaleza recursiva de Quicksort
void iterativeQuicksort(int *data, int left, int right) {
    
    int *stack;    
    stack=calloc(right - left + 1,sizeof(int*));
    int top = -1;

    stack[ ++top ] = left;
    stack[ ++top ] = right;

 
     while ( top >= 0 )
    {

        right = stack[ top-- ];
        left = stack[ top-- ];


        int p = partition( data, left, right );


        if ( p-1 > left )
        {
            stack[ ++top ] = left;
            stack[ ++top ] = p - 1;
        }

        if ( p+1 < right )
        {
            stack[ ++top ] = p + 1;
            stack[ ++top ] = right;
        }
        
}
free(stack);  
       
}

//Imprime un arreglo

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
//Funcion que participa en la distribuición 
//y llena los arreglos a distribuir

void llenado (int *arreglo,int *data,int largo,int pos)
{
    int j,l;
     l=largo;
     for (j=0; j<pos; j=j+1) 
        { 
                     
          arreglo[j]=data[l];
          l=l + 1;        
          
         }

 
}


//Funcion que une y ordena dos arreglos, previamente ordenados con Iterative Quicksort
int * merge(int * v1, int n1, int * v2, int n2)
{
  int * result = (int *)malloc((n1 + n2) * sizeof(int));
  int i = 0;
  int j = 0;
  int k;
  for (k = 0; k < n1 + n2; k++) {
    if (i >= n1) {
      result[k] = v2[j];
      j++;
    }
    else if (j >= n2) {
      result[k] = v1[i];
      i++;
    }
    else if (v1[i] < v2[j]) { // indices in bounds as i < n1 && j < n2
      result[k] = v1[i];
      i++;
    }
    else { // v2[j] <= v1[i]
      result[k] = v2[j];
      j++;
    }
  }
  return result;
}


//Funcion principal
int main(int argc, char **argv) 
{

   int i,mode,m,s,numproc,dest,n,id,step,o,me,abortar,rc,pos;
   char processor_name[MPI_MAX_PROCESSOR_NAME];
   time_t  ts, te; /* time_t is defined on <time.h> and <sys/types.h> as long */
   clock_t cs, ce; /* clock_t is defined on <time.h> and <sys/types.h> as int */     
   MPI_Status status;
   int *chunk;//Arreglo que guarda en cada proceso, su porción correspondiente
   int *maestro;//Arreglo temporal, que guarda el arreglo correspondiente al maestro
   int *other;//Arreglo que es utilizado como receptor del envio de su procesador vecino
   int *data;
   float E_cpu;
   long E_wall;
   double elapsed_time;
   FILE * file = NULL;
   MPI_Init(&argc,&argv);
   MPI_Comm_size(MPI_COMM_WORLD,&numproc);//Se obtiene el tamaño del numero de procesadores
   MPI_Comm_rank(MPI_COMM_WORLD,&id);//Se obtiene el rank del procesador
   MPI_Get_processor_name(processor_name,&me);//Se obtiene el nombre del procesador
   //printf("Procesador [%d] vivo  en %s\n",id,processor_name);
   //fflush(stdout);

   MPI_Barrier(MPI_COMM_WORLD);  //Para pasar esta barrera, todos los procesadores deben completar el procedimiento de mas arriba
      
   if (id == MASTER) 
   {
    
                
       if (strcmp(argv[1],"-S") == 0)
    mode = 0;   
    if (strcmp(argv[1],"-V") == 0)
        mode = 1;   
    //Se lee el número de datos que posee el arreglo
     
    file = fopen(argv[2], "r");
    fscanf(file, "%d", &n);
    // compute chunk size
    s = n/numproc; if (n%numproc) s++;
    // read data from file
    data = calloc(n,sizeof(int*));
    for (i = 0; i < n; i++)
         {
	fscanf(file, "%d", &(data[i]));
         }
      fclose(file); 
  

       
	chunk=calloc(s,sizeof(int*));
       
     pos=s;


   
    ts = time(NULL);
    cs = clock();
//Master distribuye el arreglo
   for (  i=1; i<numproc; i=i+1) 
	
        {
       	  dest=i; 
		      llenado(chunk,data,s,pos);
          MPI_Send(&n,1,MPI_INT,dest,TAG1,MPI_COMM_WORLD);    
          MPI_Send(&pos,1,MPI_INT,dest,TAG2,MPI_COMM_WORLD);    
          MPI_Send(chunk,pos,MPI_INT,dest,TAG3,MPI_COMM_WORLD);
          MPI_Send(&mode,1,MPI_INT,dest,TAG4,MPI_COMM_WORLD);    
          s= s + pos;
        }        
          
        
        
           int j;
          for (j=0; j<pos; j=j+1) 
        { 
                         
              chunk[j]=data[j];
                   
         }        
          
          
        //El maestro ordena su chunk         
	      iterativeQuicksort(chunk,0,pos-1);
    

 



 }
 

      


    //En esta sección, los procesos esclavos reciben su chunk correspondiente y realizan iterative Quicksort a su arreglo recibido.
      




       if (id > MASTER) 
	
       {
             
           
              
            
              
         
            MPI_Recv(&pos,1,MPI_INT,0,TAG2,MPI_COMM_WORLD, &status);    
            chunk=calloc(pos,sizeof(int*));
            MPI_Recv(chunk,pos,MPI_INT,0,TAG3,MPI_COMM_WORLD, &status);    
            MPI_Recv(&n,1,MPI_INT,0,TAG1,MPI_COMM_WORLD, &status);    
            iterativeQuicksort(chunk, 0, pos-1);
            MPI_Recv(&mode,1,MPI_INT,0,TAG4,MPI_COMM_WORLD, &status);    
            
              if(mode==1)
              {printf("El proceso %d ha ordenado su chunk \n",id);
              printData(chunk,pos);
              }
           }
    



//Union de los vecinos

step = 1;
while(step<numproc)
{
if(id%(2*step)==0)
{
if(id+step<numproc)
{
MPI_Recv(&m,1,MPI_INT,id+step,0,MPI_COMM_WORLD,&status);
other = (int *)malloc(m*sizeof(int));
MPI_Recv(other,m,MPI_INT,id+step,0,MPI_COMM_WORLD,&status);
chunk = merge(chunk,pos,other,m);
pos = pos+m;
}
}
else
{
int cerca = id-step;
MPI_Send(&pos,1,MPI_INT,cerca,0,MPI_COMM_WORLD);
MPI_Send(chunk,pos,MPI_INT,cerca,0,MPI_COMM_WORLD);
break;
}
step = step*2;
}




//resultado final

   if(id==MASTER)
{
    
    

        if(mode==1)
     {
      printf("El arreglo ordenado de largo  %d es  \n",n);
       printData(chunk,pos);
      }


	ce = clock();
    te = time(NULL);
    E_wall = (long) (te - ts);
    E_cpu = (float)(ce - cs) / CLOCKS_PER_SEC;
    printf("%d %f,%ld\n",numproc,E_cpu,E_wall);
		
}



    

MPI_Finalize();


}



