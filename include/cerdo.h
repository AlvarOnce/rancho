#pragma once
#include "animal.h"

class Cerdo : public Animal
{
public:

	Cerdo(Casilla casillaInicial, int equipo) : Animal(casillaInicial, equipo)
	{
		vida_ = 10;
		max_casillas_movidas_ = 4;
		nFrames = 5;
		especie_ = CERDO;
	}

	//void actualizar(float dt) override;
};