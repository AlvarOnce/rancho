#include "gallina.h"

void Gallina::dibujar(Renderizador* motor)
{
	motor->dibujarSprite("../assets/Sprites/gallina/gallinaSpritesheet.png", 256, 32, posx_, posy_, capaz_, 1, 8, frameActualX_, frameActualY_);
}

//void Gallina::actualizar(float dt)
//{
//	Animal::actualizar(dt);
//	
//	setState(0, 0);
//}