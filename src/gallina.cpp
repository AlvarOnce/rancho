#include "gallina.h"
#include <cmath>

void Gallina::dibujar(Renderizador* motor)
{
	motor->dibujarSprite("../assets/Sprites/gallina/gallinaSpritesheet.png", 256, 32, posx_, posy_, capaz_, 1, 8, frameActualX_, frameActualY_);
}

//void Gallina::actualizar(float dt)
//{
//	Animal::actualizar(dt);
//	
//	setState(0, 0);
//}

std::vector<Movimiento> Gallina::movimientosPosibles() const
{
    std::vector<Movimiento> movimientos;
    Casilla origen = { casillaInicial_[0], casillaInicial_[1] };

    int alcance = this->max_casillas_movidas_;

    // area de alcance alrededor del origen
    for (int f = -alcance; f <= alcance; f++)
    {
        for (int c = -alcance; c <= alcance; c++)
        {
            // descartar la casilla actual
            if (f == 0 && c == 0) continue;

            int nuevaFila = origen.fila + f;
            int nuevaCol = origen.columna + c;

            // comprobar que el movimiento no se sale del tablero
            if (nuevaFila >= 0 && nuevaFila < Constantes::FILAS_TABLERO &&
                nuevaCol >= 0 && nuevaCol < Constantes::COLUMNAS_TABLERO)
            {
                Movimiento m;
                m.origen = origen;
                m.destino = { nuevaFila, nuevaCol };
				movimientos.push_back(m); // agregar el movimiento a la lista de movimientos posibles
            }
        }
    }
    return movimientos;
}