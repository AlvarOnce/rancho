#pragma once
#include "animal.h"
#include "onda.h"

class Cabra : public Animal
{
public:

	Cabra(Casilla casillaInicial, int equipo) : Animal(casillaInicial, equipo)
	{
		vida_ = 20;
		max_casillas_movidas_ = 4;
		nFrames = 5;
		especie_ = CABRA;
		ataque_ = new Onda(7, 100.0f, 0.6f, "../assets/Sprites/menu/selector.png", 16.0f, 0.4f);
	} 

    const char* getTipoAtaque() const override { return "Onda"; }

	//void actualizar(float dt) override;
};