#include <iostream>
#include <cassert>
#include <pthread.h>
#include <semaphore.h>

using namespace std ;

// ---------------------------------------------------------------------
// constantes 
const unsigned 
  num_items  = 40 ,
  tam_vector = 10 ;

  int vector_intermedio[tam_vector];

  sem_t produce,consume;             

// ---------------------------------------------------------------------

unsigned producir_dato()
{
  static int contador = 1 ;
  cout << "producido: " << contador << endl << flush ;
  return contador++ ;
}
// ---------------------------------------------------------------------

void consumir_dato( int dato )
{
    cout << "consumido: " << dato << endl ;
}
// ---------------------------------------------------------------------

void * productor( void * )
{ int primera_libre=0;
  for( unsigned i = 0 ; i < num_items ; i++ )
  { 
    int dato=producir_dato();
    sem_wait(&produce);
    vector_intermedio[primera_libre%10]=dato;
    primera_libre++;
    sem_post(&consume);
    
  }
  cout << "Fin hebra productora\n";
  return NULL ;
}
// ---------------------------------------------------------------------

void * consumidor( void * )
{ int primera_ocupada=0;
  for( unsigned i = 0 ; i < num_items ; i++ )
  {   
    int dato;
    sem_wait(&consume);
    dato=vector_intermedio[primera_ocupada%10];
    primera_ocupada++;
    consumir_dato( dato ) ;
    sem_post(&produce);

  }
  cout << "Fin hebra consumidora\n";
  return NULL ;
}
//----------------------------------------------------------------------

int main()
{
   pthread_t prod,cons; //creacion de las dos hebras

   sem_init(&produce,0,1); //inicialmente se puede producir
   sem_init(&consume,0,0); //inicialmente no se puede consumir

   pthread_create(&prod,NULL,productor,NULL); //ejecuta productor
   pthread_create(&cons,NULL,consumidor,NULL); //ejecuta consumidor

   pthread_join(prod,NULL); 
   pthread_join(cons,NULL); 

   sem_destroy(&produce); //destruye semaforo 
   sem_destroy(&consume); //destruye semaforo
  
   return 0 ; 
}
