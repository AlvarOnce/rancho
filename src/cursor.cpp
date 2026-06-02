#include "cursor.h"

Cursor::Cursor(float col, float fil, int id) : 
    columna_(col), fila_(fil), idJugador_(id)
{
    posx_ = 141 + 11 + 22 * columna_;
    posy_ = 36 + 11 + 22 * (8-fila_);
}

void Cursor::mover(int x, int y)
{
    posx_ += 22 * x;
    posy_ += 22 * y;
    columna_ += x;
    fila_ -= y;

    //posx_ = 141 + 11 + 22 * columna;
    //posy_ = 36 + 11 + 22 * fila;

    if (columna_ < 0) { columna_ = 0;  posx_ = 141 + 11; }
    if (columna_ > 8) { columna_ = 8;  posx_ = 141 + 11 + 22 * 8; }
    if (fila_ < 0) { fila_ = 0;     posy_ = 36 + 11 + 22 * 8; }
    if (fila_ > 8) { fila_ = 8;     posy_ = 36 + 11; }
}

