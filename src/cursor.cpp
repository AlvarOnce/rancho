#include "cursor.h"
#include "renderizador.h"

void Cursor::mover(int x, int y)
{
	posx += 22*x;
	posy += 22*y;
	columna += x;
	fila -= y; // Fila de cursor aumenta al bajar, contrario a su movimiento en pantalla (que considera Y+ hacia arriba)
	if (columna < 0)	{columna = 0;		posx = 141 + 11;}
	if (columna > 8)	{columna = 8;		posx = 141 + 11 + 22 * 8;}
	if (fila < 0)		{fila = 0;			posy = 36 + 11 + 22 * 8;}
	if (fila > 8)		{fila = 8;			posy = 36 + 11;}

}

void Cursor::dibujar(Renderizador* motor)
{
	motor->dibujarSprite("../assets/Sprites/tablero/cursor.png", 32, 32, posx, posy, -5);
}