#pragma once

class Cursor 
{
    float posx_;
    float posy_;
    int idJugador_;

public:

    int fila;
    int columna;

    Cursor() : posx_(240), posy_(135), idJugador_(0), fila(4), columna(4) {}
    Cursor(float x_inicial, float y_inicial, int id);

    void mover(int x, int y);

    float getPosX() const { return posx_; }
    float getPosY() const { return posy_; }
	int getFila() const { return fila; }
	int getColumna() const { return columna; }
};