#pragma once
#include "animal.h"

class Llama : public Animal {

public:


	Llama(float posx, float posy, float capa, int vida, float xinicial, int equipo) 
		: Animal(posx, posy, capa, vida, xinicial, equipo) {

		nFrames = 5;
		setState(0, 0);

	}

	void actualizar(float dt) override;
	void dibujar(Renderizador* motor) override;
};