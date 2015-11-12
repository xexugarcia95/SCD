// *********************************************************************
// SCD. Ejemplos del seminario 1.
//
// Plantilla para el ejercicio de cálculo de PI
// *********************************************************************

#include <iostream>
#include <pthread.h>
#include "fun_tiempo.h"

using namespace std ;

// ---------------------------------------------------------------------
// constante y variables globales (compartidas entre hebras)

const unsigned long m = long(1024)*long(1024) ;  // número de muestras (un millón)
const unsigned n      = 10 ;                      // número de hebras

double resultado_parcial[n] ; // tabla de sumas parciales (una por hebra)

// ---------------------------------------------------------------------
// implementa función $f$

double f( double x )       
{ 
   return 4.0/(1+x*x) ;     // $~~~~f(x)\,=\,4/(1+x^2)$
}
// ---------------------------------------------------------------------
// cálculo secuencial

double calcular_integral_secuencial( )
{  
   double suma = 0.0 ;                      // inicializar suma
   for( unsigned long i = 0 ; i < m ; i++ ) // para cada $i$ entre $0$ y $m-1$
      suma += f( (i+0.5)/m );               // $~~~~~$ añadir $f(x_i)$ a la suma actual
   return suma/m ;                          // devolver valor promedio de $f$
}
// ---------------------------------------------------------------------
// función que ejecuta cada hebra

void * funcion_hebra( void * ih_void ) 
{  
   struct timespec inicio = ahora(); //inicio = inicio del tiempo a medir
   unsigned long ih = (unsigned long) ih_void ; // número o índice de esta hebra
   double sumap = 0.0 ; //valor inicial de la suma parcial
   

   for(unsigned long i=0;i<m;i+=n)
   {
      sumap+=f((i+0.5)/m); //incremento de la suma parcial respecto al numero de hebras que usemos
   }
   resultado_parcial[ih] = sumap ; // guardar suma parcial en vector.
   struct timespec fin = ahora(); //fin = fin del tiempo a medir
   cout << "tiempo transcurrido == " << duracion(&inicio,&fin) << " seg." << endl;
   return NULL ;
}
// ---------------------------------------------------------------------
// cálculo concurrente

double calcular_integral_concurrente( )
{  
   
   pthread_t id_hebra[n]; //creamos el vector de identicador de hebras
   for(unsigned long i=0;i<n;i++)
   {  void * arg_ptr= (void *) i;
      pthread_create(&(id_hebra[i]),NULL,funcion_hebra,arg_ptr); //iniciamos cada hebra
   }

   double res_ent=0.0; 
   void **ptr_res_ent = (void **) &res_ent;

   //join
   for(unsigned i=0;i<n;i++){
      pthread_join(id_hebra[i],ptr_res_ent);
   }

   for(unsigned long i=0;i<n;i++)
   {
      res_ent+=resultado_parcial[i];
   }

   return res_ent/m ;
}

// ---------------------------------------------------------------------

int main()
{
   
   cout << "Ejemplo 4 (cálculo de PI)" << endl ;
   double pi_sec = 0.0, pi_conc = 0.0 ;
   
   pi_sec  = calcular_integral_secuencial() ;
   pi_conc = calcular_integral_concurrente() ;
   
   cout << "valor de PI (calculado secuencialmente)  == " << pi_sec  << endl 
        << "valor de PI (calculado concurrentemente) == " << pi_conc << endl ; 
    
   return 0 ;
}
// ---------------------------------------------------------------------



// ----------------------------------------------------------------------------
