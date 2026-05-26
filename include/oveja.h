#pragma once
#include <vector>
#include "animal.h"
#include "estructuras.h"

class Oveja : public Animal 
{
public:

	Oveja(float posx, float posy, float capa, int vida, float xinicial, int equipo) : 
		  Animal(posx, posy, capa, vida, xinicial, equipo) 
	{
		max_casillas_movidas_ = 5;
		nFrames = 5;

	}

	//void actualizar(float dt) override;
	void dibujar(Renderizador* motor) override;
};