#pragma once
#include "animal.h"

class Llama : public Animal
{
public:

	Llama(Casilla casillaInicial, int equipo) : Animal(casillaInicial, equipo)
	{
		vida_ = 10;
		max_casillas_movidas_ = 6;
		nFrames = 5;
		especie_ = LLAMA;
	}

	//void actualizar(float dt) override;
};