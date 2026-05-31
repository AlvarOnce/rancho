#pragma once
#include "animal.h"
#include "disparo.h"

class Llama : public Animal
{
public:

	Llama(Casilla casillaInicial, int equipo) : Animal(casillaInicial, equipo)
	{
		vida_ = 10;
		max_casillas_movidas_ = 6;
		nFrames = 5;
		especie_ = LLAMA;
		ataque_ = new Disparo(2, 140.0f, 0.8f, "../assets/Sprites/menu/selector.png", 15.0f, 0.35f);
	}
	const char* getTipoAtaque() const override { return "Disparo"; }
	//void actualizar(float dt) override;
};