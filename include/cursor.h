#pragma once

class Cursor 
{
    float posx_;
    float posy_;
    int idJugador_;
    int fila_;
    int columna_;

public:

    Cursor() : posx_(240), posy_(135), idJugador_(0), fila_(4), columna_(4) {}
    Cursor(float col, float fil, int id);

    void mover(int x, int y);

    float getPosX() const { return posx_; }
    float getPosY() const { return posy_; }
	int getFila() const { return fila_; }
	int getColumna() const { return columna_; }
    float getIdJugador() const { return idJugador_; }
};