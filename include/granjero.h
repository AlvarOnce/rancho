#pragma once
#include "animal.h"

class Granjero : public Animal 
{
public:

	Granjero(float posx, float posy, float capa, int vida, float xinicial, int equipo) 
		: Animal(posx, posy, capa, vida, xinicial, equipo) 
	{
		nFrames = 5;

	}

	void actualizar(float dt) override;
	void dibujar(Renderizador* motor) override;
};