#pragma once
#include "animal.h"
#include "estructuras.h"

class Gallina : public Animal 
{
public:

	Gallina(float posx, float posy, float capa, int vida, float xinicial, int equipo) : Animal(posx, posy, capa, vida, xinicial, equipo) {

		max_casillas_movidas_ = 2;
		nFrames = 8;
		ataque_ = 5;

	}

	void dibujar(Renderizador* motor) override;
};