#pragma once
#include "animal.h"
#include "embestida.h"

class Oveja : public Animal
{
public:

	Oveja(Casilla casillaInicial, int equipo) : Animal(casillaInicial, equipo)
	{
		vida_ = 10;
		vida_base_ = 10;
		max_casillas_movidas_ = 3;
		nFrames = 5;
		especie_ = OVEJA;
		ataque_ = new Embestida(5, 0.2f, 0.9f,"../assets/Sprites/creditos/botella.png", 16.0f);
	}
	const char* getTipoAtaque() const override { return "Embestida"; }
	//void actualizar(float dt) override;
};