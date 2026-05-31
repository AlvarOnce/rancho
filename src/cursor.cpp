#include "cursor.h"

Cursor::Cursor(float col, float fil, int id) : 
    casilla_(fil, col), idJugador_(id)
{
    posx_ = 141 + 11 + 22 * casilla_.columna;
    posy_ = 36 + 11 + 22 * (8 - casilla_.fila);
}

void Cursor::mover(int x, int y)
{
    posx_ += 22 * x;
    posy_ += 22 * y;
    casilla_.columna += x;
    casilla_.fila -= y;

    //posx_ = 141 + 11 + 22 * columna;
    //posy_ = 36 + 11 + 22 * fila;

    /*if (casilla_.columna < 0) { casilla_.columna = 0;  posx_ = 141 + 11; }
    if (casilla_.columna > 8) { casilla_.columna = 8;  posx_ = 141 + 11 + 22 * 8; }
    if (casilla_.fila < 0) { casilla_.fila = 0;     posy_ = 36 + 11 + 22 * 8; }
    if (casilla_.fila > 8) { casilla_.fila = 8;     posy_ = 36 + 11; }*/
}

