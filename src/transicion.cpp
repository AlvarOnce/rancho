#include "transicion.h"

void Transicion::actualizar(float dt) {

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

void Transicion::dibujar(Renderizador* motor) {

	if (tamano>0)
		motor->dibujarSprite("../assets/Sprites/menu/transicionAgujero.png", tamano, tamano, 480 / 2, 270 / 2, -10, 1, 1, 0, 0, false); // agujero transparente

		motor->dibujarSprite("../assets/Sprites/menu/transicionFondo.png", 480, 270, 480 / 2, 270 / 2, -10, 1, 1, 0, 0, false); // fondo negro

}