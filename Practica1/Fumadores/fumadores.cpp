#include <iostream>
#include <cassert>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>      // incluye "time(....)"
#include <unistd.h>    // incluye "usleep(...)"
#include <stdlib.h>    // incluye "rand(...)" y "srand"

using namespace std;

// ----------------------------------------------------------------------------
	//semaforos
	sem_t fuma1,fuma2,fuma3,estanco;

	bool esCierto=true;

// ----------------------------------------------------------------------------

void fumar() // función que simula la acción de fumar  como un retardo aleatorio de la hebra

{ 
   static bool primera_vez = true ;
   if ( primera_vez )
   {   primera_vez = false ;
      srand( time(NULL) );    
   }
   // calcular un numero aleatorio de milisegundos (entre 1/10 y 2 segundos)
   const unsigned miliseg = 100U + (rand() % 1900U) ; 

   // retraso bloqueado durante 'miliseg' milisegundos
   usleep( 1000U*miliseg ); 
}
// ----------------------------------------------------------------------------

void * estanquero(void* p)
{
	int variable=1;
	
	for(int i=0;i<variable;i++){
		int r_number=0;
		
		sem_wait(&estanco);
		r_number= rand() % 3 + 1; //1 == tabaco , 2 == papel, 3 == cerillas
		if(r_number == 1){
			cout << "el estanquero produce tabaco\n";
			sem_post(&fuma2);
		}else if(r_number == 2){
				cout << "el estanquero produce papel\n";
				sem_post(&fuma3);
			}else if(r_number == 3){
					cout << "el estanquero produce cerillas\n";
					sem_post(&fuma1);
				}
		variable++;
	}

	
}
// ----------------------------------------------------------------------------
void * fumador1(void* p)
{	while(esCierto){
	sem_wait(&fuma1);
	cout << "El fumador 1 fuma\n";
	fumar();
	sem_post(&estanco);
	}
	
}
// ----------------------------------------------------------------------------
void * fumador2(void* p)
{	while(esCierto){
	sem_wait(&fuma2);
	cout << "El fumador 2 fuma\n";
	fumar();
	sem_post(&estanco);
	}
	
}
// ----------------------------------------------------------------------------
void * fumador3(void* p)
{	while(esCierto){
	sem_wait(&fuma3);
	cout << "El fumador 3 fuma\n";
	fumar();
	sem_post(&estanco);
	}
}
// ----------------------------------------------------------------------------


int main()
{
  srand( time(NULL) );   
  pthread_t f1,f2,f3,es;

  sem_init(&fuma1,0,0);
  sem_init(&fuma2,0,0);
  sem_init(&fuma3,0,0);
  sem_init(&estanco,0,1);

  pthread_create(&es,NULL,estanquero,NULL);
  pthread_create(&f1,NULL,fumador1,NULL);
  pthread_create(&f2,NULL,fumador2,NULL);
  pthread_create(&f3,NULL,fumador3,NULL);
  

  
  pthread_join(es,NULL);


  sem_destroy(&estanco);
  sem_destroy(&fuma1);
  sem_destroy(&fuma2);
  sem_destroy(&fuma3);
  
   
  cout << "FIN\n";
}
