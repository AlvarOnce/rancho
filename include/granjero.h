#pragma once
#include "animal.h"

class Granjero : public Animal
{
public:

	Granjero(Casilla casillaInicial, int equipo) : Animal(casillaInicial, equipo)
	{
		vida_ = 10;
		max_casillas_movidas_ = 10;
		nFrames = 5;
		especie_ = GRANJERO;
	}

	//void actualizar(float dt) override;
};