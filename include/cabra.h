#pragma once
#include "animal.h"
#include "embestida.h"

class Cabra : public Animal
{
public:

	Cabra(Casilla casillaInicial, int equipo) : Animal(casillaInicial, equipo)
	{
		vida_ = 10;
		max_casillas_movidas_ = 4;
		nFrames = 5;
		especie_ = CABRA;
		ataque_ = new Embestida(8, 0.3f, 1.2f,"../assets/Sprites/creditos/botella.png", 20.0f);
	} 

    const char* getTipoAtaque() const override { return "Embestida"; }

	//void actualizar(float dt) override;
};