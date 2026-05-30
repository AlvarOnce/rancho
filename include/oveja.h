#pragma once
#include "animal.h"

class Oveja : public Animal
{
public:

	Oveja(Casilla casillaInicial, int equipo) : Animal(casillaInicial, equipo)
	{
		vida_ = 10;
		max_casillas_movidas_ = 3;
		nFrames = 5;
		especie_ = OVEJA;
	}

	//void actualizar(float dt) override;
};