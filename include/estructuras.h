#pragma once

struct Casilla {
	int fila;
	int columna;
};

struct Movimiento {
	Casilla origen;
	Casilla destino;
};

struct Animacion {
	float timer;
	float msStep;
	int nFrames;
};