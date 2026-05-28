#include "transicion.h"

void Transicion::actualizar(float dt) 
{
	switch (estado) {

	case ESPERANDO:
		if (activo)
			estado = CERRANDO;
		break;

	case CERRANDO:
			tamano -= 8;
		if (tamano < 0)
			estado = CERRADO;
		break;

	case CERRADO:
		tamano -= 5;
		if (tamano < -40)
			estado = ABRIENDO;
		break;

	case ABRIENDO:
			tamano += 8;
		if (tamano > 600)
		{
			activo = false;
			estado = ESPERANDO;
		}
			break;
	}
}