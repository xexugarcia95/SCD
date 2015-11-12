//ejemplo de paso de parametros a hebras

#include <iostream>
#include <pthread.h>
using namespace std;

const unsigned num_hebras=10;

//funcion que ejecuta cada hebra(calculo arbitrario)

void* funcion(void *arg_ptr)
{
	unsigned long arg_ent = (unsigned long)arg_ptr; //convertir puntero en entero
	unsigned long res_ent;
	void *		  res_ptr;

	//aqui se incluye cualquier calculo que se le asigne
	//un valor a res_ent usando el valor de arg_ent
	res_ent = arg_ent*arg_ent;

	//valor ya calculado, devolver valor como puntero
	res_ptr = (void *) res_ent; //convertir entero en puntero
	return res_ptr; //devolver resultado
}

	int main()
	{
		pthread_t id_hebra[num_hebras]; //vector de identificadores de hebra

		//lanzar las hebras
		for(unsigned long i=0;i<num_hebras;i++)
		{	void * arg_ptr= (void *) i;
			pthread_create(&(id_hebra[i]),NULL,funcion,arg_ptr);
		}

		//esperar a hebras e imprimir resultado
		unsigned long res_ent; //guarda el resultado de cada hebra
		void** ptr_res_ent= (void**) &res_ent; //puntero a variable con resultado

		for(unsigned i=0;i<num_hebras;i++)
		{
			pthread_join(id_hebra[i],ptr_res_ent); //asigna a res_ent
			cout <<  "func(" << i << ") == " << res_ent << endl;
		}
	}