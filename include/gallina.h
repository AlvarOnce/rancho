#pragma once
#include "animal.h"
#include "disparo.h"

class Gallina : public Animal 
{
public:

	Gallina(Casilla casillaInicial, int equipo) : Animal(casillaInicial, equipo)
	{
		vida_ = 10;
		vida_base_ = 10;
		max_casillas_movidas_ = 2;
		nFrames = 8;
		especie_ = GALLINA;
		ataque_ = new Disparo(2, 140.0f, 0.8f, "../assets/Sprites/menu/selector.png", 15.0f, 0.35f);
	}

	const char* getTipoAtaque() const override { return "Disparo";}
};
	