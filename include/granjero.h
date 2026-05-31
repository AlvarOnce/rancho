#pragma once
#include "animal.h"
#include "golpe.h"

class Granjero : public Animal
{
public:

	Granjero(Casilla casillaInicial, int equipo) : Animal(casillaInicial, equipo)
	{
		vida_ = 10;
		max_casillas_movidas_ = 10;
		nFrames = 5;
		especie_ = GRANJERO;
		ataque_ = new Golpe(4, 100.0f, 0.6f, "../assets/Sprites/menu/selector.png", 16.0f, 0.4f);
	}
	const char* getTipoAtaque() const override { return "Golpe"; }
	//void actualizar(float dt) override;
};