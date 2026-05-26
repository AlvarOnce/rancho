#include "oveja.h"

void Oveja::dibujar(Renderizador* motor)
{
	motor->dibujarSprite("../assets/Sprites/oveja/ovejaSpritesheet.png", 256, 64, posx_, posy_, capaz_, 2, 8, frameActualX_, frameActualY_);
}

//void Oveja::actualizar(float dt)
//{
//	Animal::actualizar(dt);
//	
//	setState(0, 0);
//}