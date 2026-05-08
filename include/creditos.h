#pragma once
#include "renderizador.h"

class BotellaCreditos
{

	float timer = 0;
	float msStep = 100;
	int nFrames = 8;
	bool loop = false;

public:

	float posx_ = 364;
	const float posy_ = 25;
	int frameActualX_ = 0;
	int frameActualY_ = 1;

	void animar(float dt);

};

class GallinaCreditos {

	float timer = 0;
	float msStep = 100;
	int nFrames = 8;

public:

	const float posx_ = 40;
	const float posy_ = 37;
	int frameActualX_ = 0;
	int frameActualY_ = 1;

	void animar(float dt);
};

class Creditos {

private:

	BotellaCreditos botella;
	GallinaCreditos gallina;
	float posx_;
	float posy_;
	const float posFinal_ = -246;
	float capaz_;
	bool fin_;

public:

	void reset();
	Creditos() { reset(); }
	bool getFinalizado() const { return fin_; }

	void actualizar(float dt);
	void dibujar(Renderizador* motor);
};