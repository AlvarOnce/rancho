#pragma once
#include "animal.h"
#include "onda.h"

class Cerdo : public Animal
{
public:

	Cerdo(Casilla casillaInicial, int equipo) : Animal(casillaInicial, equipo)
	{
		vida_ = 10;
		vida_base_ = 10;
		max_casillas_movidas_ = 4;
		nFrames = 5;
		especie_ = CERDO;
		ataque_ = new Onda(3, 100.0f, 1.0f,"../assets/Sprites/menu/selector.png", 20.0f, 0.6f);
	}
	const char* getTipoAtaque() const override { return "Onda"; }
	//void actualizar(float dt) override;
};