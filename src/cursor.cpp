#include "cursor.h"
#include "renderizador.h"

Cursor::Cursor(float x_inicial, float y_inicial, int id)
    : posx_(x_inicial), posy_(y_inicial), idJugador_(id)
{
    columna = (int)((x_inicial - 141.0f - 11.0f) / 22.0f);
    fila = 8 - (int)((y_inicial - 36.0f - 11.0f) / 22.0f);
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

void Cursor::dibujar(Renderizador* motor)
{
    if (idJugador_ == 0)
        motor->dibujarSprite("../assets/Sprites/tablero/cursor.png", 32, 32, posx_, posy_, -5.0f);
    else
        motor->dibujarSprite("../assets/Sprites/tablero/cursor.png", 32, 32, posx_, posy_, -5.0f);
}