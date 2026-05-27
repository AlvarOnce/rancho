#include "cursor.h"

Cursor::Cursor(float col, float fil, int id) : 
    columna(col), fila(fil), idJugador_(id)
{
    posx_ = 141 + 11 + 22 * columna;
    posy_ = 36 + 11 + 22 * fila;
}

void Cursor::mover(int x, int y)
{
    posx_ += 22 * x;
    posy_ += 22 * y;
    columna += x;
    fila -= y;

    if (columna < 0) { columna = 0;  posx_ = 141 + 11; }
    if (columna > 8) { columna = 8;  posx_ = 141 + 11 + 22 * 8; }
    if (fila < 0) { fila = 0;     posy_ = 36 + 11 + 22 * 8; }
    if (fila > 8) { fila = 8;     posy_ = 36 + 11; }
}

