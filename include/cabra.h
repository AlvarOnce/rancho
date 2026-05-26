#pragma once
#include "animal.h"

class Cabra: public Animal 
{
public:

	Cabra(float posx,float posy, float capa, int vida, float xinicial, int equipo ) 
		: Animal(posx,posy,capa,vida,xinicial,equipo) 
	{
		max_casillas_movidas_ = 4;
		nFrames = 5;
	}

	//void actualizar(float dt) override;
	void dibujar(Renderizador* motor) override;
};