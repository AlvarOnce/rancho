#pragma once
#include "estructuras.h"
class Cursor 
{
    float posx_;
    float posy_;
    int idJugador_;
    Casilla casilla_;

public:

    Cursor() : posx_(240), posy_(135), idJugador_(0), casilla_{4, 4} {}
    Cursor(float col, float fil, int id);

    void mover(int x, int y);

    float getPosX() const { return posx_; }
    float getPosY() const { return posy_; }
	int getFila() const { return casilla_.fila; }
	int getColumna() const { return casilla_.columna; }
    float getIdJugador() const { return idJugador_; }
};