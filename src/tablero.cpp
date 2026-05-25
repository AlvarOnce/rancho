#include "tablero.h"
//cursorJ1_(141.0f + 11.0f, 36.0f + 11.0f + 22.0f * 8.0f, 0),
//cursorJ2_(141.0f + 11.0f + 22.0f * 8.0f, 36.0f + 11.0f, 1)

Tablero::Tablero(Jugador* jugador1, Jugador* jugador2) : 
cursorJ1_(141.0f + 11.0f, 36.0f + 11.0f + 22.0f * 8.0f, 0),
    cursorJ2_(141.0f + 11.0f + 22.0f * 8.0f, 36.0f + 11.0f, 1)
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

Tablero::~Tablero() {} // las piezas se destruyen en el jugador, no en el tablero

Cursor& Tablero::getCursorActivo()
{
    return turno_actual_ == 0 ? cursorJ1_ : cursorJ2_;
}

Jugador* Tablero::getJugadorActivo()
{
    return jugadores_[turno_actual_];
}

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

    letreroTurnos_.posx = 102 + turno_actual_ * 273;
    letreroTurnos_.animar(dt);
}

void Tablero::dibujar(Renderizador* motor)
{
    motor->dibujarSprite("../assets/Sprites/tablero/tableroFondo.png", 512, 512, 480 / 2, 270 / 2, -1);
    motor->dibujarSprite("../assets/Sprites/tablero/tablero.png", 256, 256, 480 / 2, 270 / 2, -2);
    motor->dibujarSprite("../assets/Sprites/tablero/turnos.png", 256, 128, letreroTurnos_.posx, 270 / 2, -5, 4, 8, letreroTurnos_.frameActualX_, letreroTurnos_.frameActualY_);

    for (int i = 0; i < Constantes::FILAS_TABLERO; i++)
        for (int j = 0; j < Constantes::COLUMNAS_TABLERO; j++)
            if (casillas_[i][j] != nullptr)
                casillas_[i][j]->dibujar(motor);

    if (getJugadorActivo()->tienePiezaAgarrada())
        getJugadorActivo()->getPiezaSeleccionada()->dibujar(motor);

    cursorJ1_.dibujar(motor);
    cursorJ2_.dibujar(motor);

    if (getCursorActivo().getPosX() > 150 && getCursorActivo().getPosX() < 170)
        tarjeta.dibujar(motor);
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

            bool movimiento_valido = false;
            if (dx == 0 && dy == 1)  movimiento_valido = pieza->mover(TABLERO, U);
            if (dx == 0 && dy == -1) movimiento_valido = pieza->mover(TABLERO, D);
            if (dx == -1 && dy == 0) movimiento_valido = pieza->mover(TABLERO, L);
            if (dx == 1 && dy == 0)  movimiento_valido = pieza->mover(TABLERO, R);

            if (movimiento_valido)
                cursor.mover(dx, dy);
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

        if (casilla == nullptr && !jugadorActivo->tienePiezaAgarrada()) {
            // casilla vacía y sin pieza agarrada, no hace nada
        }
        else if (casilla != nullptr && !jugadorActivo->tienePiezaAgarrada()) {

            if (casilla->equipo_ == jugador)
            {
                jugadorActivo->agarrarPieza(casilla);
                casillas_[cursor.fila][cursor.columna] = nullptr;
            }
        }
        else if (casilla == nullptr && jugadorActivo->tienePiezaAgarrada()) {

            casillas_[cursor.fila][cursor.columna] = jugadorActivo->getPiezaSeleccionada();
            jugadorActivo->soltarPieza();

            turno_actual_ = (turno_actual_ == 0) ? 1 : 0;
            letreroTurnos_.setState(0, turno_actual_);
        }
        else if (casilla != nullptr && jugadorActivo->tienePiezaAgarrada()) {
            // colisión con enemigo, se gestiona desde Juego con getHayColision()
        }
    }
}

void Tablero::actualizarColision()
{
    Cursor& cursor = getCursorActivo();
    Jugador* jugadorActivo = getJugadorActivo();

    if (casillas_[cursor.fila][cursor.columna] != nullptr
        && jugadorActivo->tienePiezaAgarrada()
        && casillas_[cursor.fila][cursor.columna]->equipo_ != jugadorActivo->getEquipo())
        hay_colision_ = true;
    else
        hay_colision_ = false;
}

void Letrero::animar(float dt)
{
    timer = timer + dt;
    if (timer > msStep)
    {
        if (frameActualX_ < nFrames - 1) frameActualX_++;
        else if (loop) frameActualX_ = 0;
        timer = timer - msStep;
    }
}

void Letrero::setState(int frameX, int frameY)
{
    frameActualX_ = frameX;
    frameActualY_ = frameY;
    loop = false;
}