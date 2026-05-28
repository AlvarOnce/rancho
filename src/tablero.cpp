#include "tablero.h"
#include <cmath>

Tablero::Tablero(Jugador* jugador1, Jugador* jugador2)
{
    jugadores_[0] = jugador1;
    jugadores_[1] = jugador2;
    inicializarTablero();

    for (int j = 0; j < 2; j++)
        for (int i = 0; i < Constantes::FILAS_TABLERO; i++)
            casillas_[i][j] = jugadores_[0]->getAnimales()[j * Constantes::FILAS_TABLERO + i];

    for (int j = 0; j < 2; j++)
        for (int i = 0; i < Constantes::FILAS_TABLERO; i++)
            casillas_[i][8 - j] = jugadores_[1]->getAnimales()[j * Constantes::FILAS_TABLERO + i];
}

Tablero::~Tablero() {}

void Tablero::inicializarTablero()
{
    for (int i = 0; i < Constantes::FILAS_TABLERO; i++)
    {
        for (int j = 0; j < Constantes::COLUMNAS_TABLERO; j++)
        {
            casillas_[i][j] = nullptr;

            if ((i + j) % 2 == 0)
                color_casilla_[i][j] = CASILLA_LUZ;
            else
                color_casilla_[i][j] = CASILLA_OSCURA;
        }
    }
}

void Tablero::actualizar(float dt)
{
    for (int i = 0; i < Constantes::FILAS_TABLERO; i++)
        for (int j = 0; j < Constantes::COLUMNAS_TABLERO; j++)
            if (casillas_[i][j] != nullptr)
                casillas_[i][j]->actualizar(dt);

    if (getJugadorActivo()->tienePiezaAgarrada())
        getJugadorActivo()->getPiezaSeleccionada()->actualizar(dt);

    actualizarColision();

    setLetreroPosX(102 + turno_actual_ * 273);
    letreroTurnos_.animar(dt);
}

void Tablero::recibirMovimiento(int jugador, int dx, int dy)
{
    if (casillas_[8][0] != nullptr && casillas_[8][0]->getIntroTablero()) return;

    Cursor& cursor = jugador == 0 ? cursorJ1_ : cursorJ2_;

    if (jugador == turno_actual_)
    {
        Jugador* jugadorActivo = getJugadorActivo();

        if (!jugadorActivo->tienePiezaAgarrada())
        {
            cursor.mover(dx, dy);
        }
        else
        {
            Animal* pieza = jugadorActivo->getPiezaSeleccionada();
            if (pieza->getEnMovimiento()) return;

            pieza->mover(TABLERO, dx, dy);
        }
    }
}

void Tablero::seleccionarPieza(int jugador)
{
    if (turno_actual_ == jugador)
    {
        Cursor& cursor = getCursorActivo();
        Jugador* jugadorActivo = getJugadorActivo();
        Animal* casilla = casillas_[cursor.fila][cursor.columna];

        // CASO 1: LEVANTAR UNA PIEZA
        if (!jugadorActivo->tienePiezaAgarrada() && casilla != nullptr)
        {
            if (casilla->equipo_ == jugador)
            {
                casilla->casillaInicial_[0] = cursor.fila;
                casilla->casillaInicial_[1] = cursor.columna;

                jugadorActivo->agarrarPieza(casilla);
                casillas_[cursor.fila][cursor.columna] = nullptr;
            }
        }
        // CASO 2: SOLTAR UNA PIEZA
        else if (jugadorActivo->tienePiezaAgarrada())
        {
            Animal* pieza = jugadorActivo->getPiezaSeleccionada();

            if (pieza->getEnMovimiento()) return;

            Movimiento m;
            m.origen.fila = pieza->casillaInicial_[0];
            m.origen.columna = pieza->casillaInicial_[1];
            m.destino.columna = std::round((pieza->getPosX() - 152.0f) / 22.0f);
            m.destino.fila = 8 - std::round((pieza->getPosY() - 47.0f) / 22.0f);

            if (esMovimientoLegal(m))
            {
                // CORRECCIÓN: guardar el rival ANTES de llamar a mover(),
                // porque mover() sobreescribe casillas_[destino] con la pieza
                // que se mueve, haciendo que animal1 == animal2.
                Animal* rival = casillas_[m.destino.fila][m.destino.columna];

                mover(m);

                while (cursor.columna < m.destino.columna) cursor.mover(1, 0);
                while (cursor.columna > m.destino.columna) cursor.mover(-1, 0);
                while (cursor.fila > m.destino.fila)    cursor.mover(0, 1);
                while (cursor.fila < m.destino.fila)    cursor.mover(0, -1);

                if (getHayColision())
                {
                    Animal* animal1 = pieza;
                    Animal* animal2 = rival; // ahora sí son dos animales distintos

                    // Asignamos según equipo: [0] siempre equipo 0, [1] siempre equipo 1
                    if (animal1->equipo_ == 0 && animal2->equipo_ == 1) {
                        animalesEnBatalla[0] = animal1;
                        animalesEnBatalla[1] = animal2;
                    }
                    else {
                        animalesEnBatalla[0] = animal2;
                        animalesEnBatalla[1] = animal1;
                    }

                    enBatalla = true;
                }

                jugadorActivo->soltarPieza();
                turno_actual_ = (turno_actual_ == 0) ? 1 : 0;
                letreroTurnos_.setState(0, turno_actual_);
            }
            else
            {
                // MOVIMIENTO ILEGAL: volver a colocar la pieza en su posición original
                casillas_[m.origen.fila][m.origen.columna] = pieza;

                float origX = 141.0f + 11.0f + (22.0f * m.origen.columna);
                float origY = 36.0f + 11.0f + (22.0f * (8 - m.origen.fila));
                pieza->setPosX(origX);
                pieza->setPosy(origY);

                pieza->setVelX(0);
                pieza->setVelY(0);
                pieza->setEnMovimiento(false);
                pieza->avanzando_casilla_ = 0;
                pieza->casillas_movidas_ = 0;
                pieza->casillas_movidas_x_ = 0;
                pieza->casillas_movidas_y_ = 0;

                jugadorActivo->soltarPieza();
            }
        }
    }
}

void Tablero::actualizarColision()
{
    Jugador* jugadorActivo = getJugadorActivo();

    if (jugadorActivo->tienePiezaAgarrada())
    {
        Animal* pieza = jugadorActivo->getPiezaSeleccionada();

        int destCol = std::round((pieza->getPosX() - 152.0f) / 22.0f);
        int destFila = 8 - std::round((pieza->getPosY() - 47.0f) / 22.0f);

        if (destFila >= 0 && destFila < Constantes::FILAS_TABLERO &&
            destCol >= 0 && destCol < Constantes::COLUMNAS_TABLERO)
        {
            if (casillas_[destFila][destCol] != nullptr &&
                casillas_[destFila][destCol]->equipo_ != jugadorActivo->getEquipo())
            {
                hay_colision_ = true;
                return;
            }
        }
    }

    hay_colision_ = false;
}

void Letrero::animar(float dt)
{
    timer += dt;
    if (timer > msStep)
    {
        if (frameActualX_ < nFrames - 1) frameActualX_++;
        else if (loop) frameActualX_ = 0;
        timer -= msStep;
    }
}

void Letrero::setState(int frameX, int frameY)
{
    frameActualX_ = frameX;
    frameActualY_ = frameY;
    loop = false;
}

bool Tablero::esMovimientoLegal(const Movimiento& m) const
{
    if (m.destino.fila < 0 || m.destino.fila >= Constantes::FILAS_TABLERO ||
        m.destino.columna < 0 || m.destino.columna >= Constantes::COLUMNAS_TABLERO)
        return false;

    Jugador* jugadorActivo = jugadores_[turno_actual_];
    if (!jugadorActivo || !jugadorActivo->tienePiezaAgarrada()) return false;

    Animal* pieza = jugadorActivo->getPiezaSeleccionada();
    if (!pieza) return false;

    Animal* casillaDestino = casillas_[m.destino.fila][m.destino.columna];
    if (casillaDestino != nullptr && casillaDestino->equipo_ == turno_actual_) return false;

    std::vector<Movimiento> permitidos = pieza->movimientosPosibles();

    for (const auto& mov : permitidos)
        if (mov.destino.fila == m.destino.fila && mov.destino.columna == m.destino.columna)
            return true;

    return false;
}

void Tablero::mover(const Movimiento& m)
{
    Jugador* jugadorActivo = getJugadorActivo();
    Animal* pieza = jugadorActivo->getPiezaSeleccionada();

    casillas_[m.destino.fila][m.destino.columna] = pieza;

    float nuevaPosX = 141.0f + 11.0f + (22.0f * m.destino.columna);
    float nuevaPosY = 36.0f + 11.0f + (22.0f * (8 - m.destino.fila));
    pieza->setPosX(nuevaPosX);
    pieza->setPosy(nuevaPosY);

    pieza->setVelX(0);
    pieza->setVelY(0);
    pieza->setEnMovimiento(false);
    pieza->avanzando_casilla_ = 0;
    pieza->casillas_movidas_ = 0;
    pieza->casillas_movidas_x_ = 0;
    pieza->casillas_movidas_y_ = 0;
}