#pragma once

namespace Constantes 
{
	const float ANCHO_VENTANA = 480.0f;
	const float ALTO_VENTANA = 270.0f;
	const float ASPECT_RATIO = ANCHO_VENTANA / ALTO_VENTANA;
	inline constexpr int NUMERO_ANIMALES = 18;
	inline constexpr int FILAS_TABLERO = 9;
	inline constexpr int COLUMNAS_TABLERO = 9;

}

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