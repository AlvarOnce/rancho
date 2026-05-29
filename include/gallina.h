#pragma once
#include "animal.h"
#include "disparo.h"

class Gallina : public Animal 
{
public:

	Gallina(Casilla casillaInicial, int equipo) : Animal(casillaInicial, equipo)
	{
		vida_ = 20;
		max_casillas_movidas_ = 2;
		nFrames = 8;
		especie_ = GALLINA;
		ataque_ = new Disparo(10, 140.0f, 0.8f, "../assets/Sprites/creditos/botella.png", 20.0f, 0.35f);
	}

	const char* getTipoAtaque() const override { return "Disparo";}
};
	