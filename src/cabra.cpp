#include "cabra.h"

void Cabra::dibujar(Renderizador* motor) 
{
	motor->dibujarSprite("../assets/Sprites/cabra/cabraSpritesheet.png", 256, 128, posx_, posy_, capaz_, 4, 8, frameActualX_, frameActualY_);
}

//void Cabra::actualizar(float dt)
//{
//	Animal::actualizar(dt);
// 
//}