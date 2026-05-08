#pragma once
#include "renderizador.h"

class PalomaCreditos {

	float timer = 0;
	float msStep = 100;
	int nFrames = 5;

public:

	float posx_ = 55;
	float posy_ = 315;
	int frameActualX_ = 0;
	int frameActualY_ = 2;

	void animar(float dt);
	void setState(int frameX, int frameY);
};

class Controles {

private:

	PalomaCreditos paloma;
	float posx_;
	float posy_;
	const float posFinal_ = -246;
	float capaz_;
	bool fin_;

public:

	void reset();
	Controles() { reset(); }
	bool getFinalizado() const { return fin_; }

	void actualizar(float dt);
	void dibujar(Renderizador* motor);
};