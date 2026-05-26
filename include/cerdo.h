#pragma once
#include "animal.h"

class Cerdo : public Animal 
{
public:

	Cerdo(float posx, float posy, float capa, int vida, float xinicial, int equipo) 
		: Animal(posx, posy, capa, vida, xinicial, equipo) 
	{
		nFrames = 6;
		max_casillas_movidas_ = 5;
		
	}

	void dibujar(Renderizador* motor) override;
};