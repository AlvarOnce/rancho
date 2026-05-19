#include "tablero.h"

Tablero::Tablero(Jugador* jugador1, Jugador* jugador2)
    : cursorJ1_(141.0f + 11.0f, 36.0f + 11.0f + 22.0f * 8.0f, 0),
    cursorJ2_(141.0f + 11.0f + 22.0f * 8.0f, 36.0f + 11.0f, 1)
{
    jugador1_ = jugador1;
    jugador2_ = jugador2;
    inicializarTablero();

    for (int j = 0; j < 2; j++)
        for (int i = 0; i < FILAS; i++)
            casillas[i][j] = jugador1->getAnimales()[j * FILAS + i];

    for (int j = 0; j < 2; j++)
        for (int i = 0; i < FILAS; i++)
            casillas[i][8 - j] = jugador2->getAnimales()[j * FILAS + i];
}

Tablero::~Tablero() {}

Cursor& Tablero::getCursorActivo()
{
    return turno_actual == 0 ? cursorJ1_ : cursorJ2_;
}

Jugador* Tablero::getJugadorActivo()
{
    return turno_actual == 0 ? jugador1_ : jugador2_;
}

void Tablero::inicializarTablero()
{
    for (int i = 0; i < FILAS; i++)
    {
        for (int j = 0; j < COLUMNAS; j++)
        {
            casillas[i][j] = nullptr;

            if ((i + j) % 2 == 0)
                color_casilla[i][j] = CASILLA_LUZ;
            else
                color_casilla[i][j] = CASILLA_OSCURA;
        }
    }

    turno_actual = BANDO_LUZ;
}

void Tablero::actualizar(float dt)
{
    for (int i = 0; i < FILAS; i++)
        for (int j = 0; j < COLUMNAS; j++)
            if (casillas[i][j] != nullptr)
                casillas[i][j]->actualizar(dt);

    if (getJugadorActivo()->tienePiezaAgarrada())
        getJugadorActivo()->getPiezaSeleccionada()->actualizar(dt);

    actualizarColision();

    letreroTurnos.posx = 102 + turno_actual * 273;
    letreroTurnos.animar(dt);
}

void Tablero::dibujar(Renderizador* motor)
{
    motor->dibujarSprite("../assets/Sprites/tablero/tableroFondo.png", 512, 512, 480 / 2, 270 / 2, -1);
    motor->dibujarSprite("../assets/Sprites/tablero/tablero.png", 256, 256, 480 / 2, 270 / 2, -2);
    motor->dibujarSprite("../assets/Sprites/tablero/turnos.png", 256, 128, letreroTurnos.posx, 270 / 2, -5, 4, 8, letreroTurnos.frameActualX_, letreroTurnos.frameActualY_);

    for (int i = 0; i < FILAS; i++)
        for (int j = 0; j < COLUMNAS; j++)
            if (casillas[i][j] != nullptr)
                casillas[i][j]->dibujar(motor);

    if (getJugadorActivo()->tienePiezaAgarrada())
        getJugadorActivo()->getPiezaSeleccionada()->dibujar(motor);

    cursorJ1_.dibujar(motor);
    cursorJ2_.dibujar(motor);

    if (getCursorActivo().getPosX() > 150 && getCursorActivo().getPosX() < 170)
        tarjeta.dibujar(motor);
}

void Tablero::recibirMovimiento(int jugador, int dx, int dy)
{
    if (casillas[8][0] != nullptr && casillas[8][0]->getIntroTablero()) return;

    Cursor& cursor = jugador == 0 ? cursorJ1_ : cursorJ2_;

    if (jugador == turno_actual)
    {
        Jugador* jugadorActivo = getJugadorActivo();

        if (!jugadorActivo->tienePiezaAgarrada())
        {
            cursor.mover(dx, dy);
        }
        else
        {
            std::cout << "tengo pieza agarrada" << std::endl;
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
    if (turno_actual == jugador)
    {
        Cursor& cursor = getCursorActivo();
        std::cout << "seleccionando en fila: " << cursor.fila<< " col: " << cursor.columna<< " casilla: " << casillas[cursor.fila][cursor.columna]<< std::endl;
        Jugador* jugadorActivo = getJugadorActivo();
        Animal* casilla = casillas[cursor.fila][cursor.columna];

        if (casilla == nullptr && !jugadorActivo->tienePiezaAgarrada()) {
            // casilla vacía y sin pieza agarrada, no hace nada
        }
        else if (casilla != nullptr && !jugadorActivo->tienePiezaAgarrada()) {

            if (casilla->equipo_ == jugador)
            {
                jugadorActivo->agarrarPieza(casilla);
                casillas[cursor.fila][cursor.columna] = nullptr;
            }
        }
        else if (casilla == nullptr && jugadorActivo->tienePiezaAgarrada()) {

            casillas[cursor.fila][cursor.columna] = jugadorActivo->getPiezaSeleccionada();
            jugadorActivo->soltarPieza();

            turno_actual = (turno_actual == 0) ? 1 : 0;
            letreroTurnos.setState(0, turno_actual);
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

    if (casillas[cursor.fila][cursor.columna] != nullptr
        && jugadorActivo->tienePiezaAgarrada()
        && casillas[cursor.fila][cursor.columna]->equipo_ != jugadorActivo->getEquipo())
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