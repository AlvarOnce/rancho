#pragma once
#include <cmath>

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

struct Vector2D {
	float x;
	float y;

	Vector2D(float _x = 0.0f, float _y = 0.0f) : x(_x), y(_y) {}
	float modulo() const { return std::sqrtf(x * x + y * y); }

	Vector2D operator+(const Vector2D& v) const { return Vector2D(x + v.x, y + v.y); }
	Vector2D operator*(float escalar) const { return Vector2D(x * escalar, y * escalar); }
	void operator+=(const Vector2D& v) { x += v.x; y += v.y; }
};