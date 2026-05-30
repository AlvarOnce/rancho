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

    // inicializar todos los hechizos como disponibles
    for (int j = 0; j < 2; j++) {
        for (int h = 1; h <= 4; h++) {
            hechizoDisponible_[j][h] = true; // true = hechizo disponible, cuando se gasta se pone a false
        }
    }
}

Tablero::~Tablero() {} // las piezas se destruyen en el jugador, no en el tablero

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
                casillas_[i][j]->actualizarEnTablero(dt);

    if (getJugadorActivo()->tienePiezaAgarrada())
        getJugadorActivo()->getPiezaSeleccionada()->actualizarEnTablero(dt);

    actualizarColision();

    setLetreroPosX(102 + turno_actual_ * 273);
    letreroTurnos_.animar(dt);

    determinarGanador();
}

void Tablero::recibirMovimiento(int jugador, int dx, int dy) // dx y dy son -1, 0 o 1 dependiendo de la dirección del movimiento
{
    if (casillas_[8][0] != nullptr && casillas_[8][0]->getIntroTablero()) return;

    Cursor& cursor = jugador == 0 ? cursorJ1_ : cursorJ2_;

    if (jugador == turno_actual_)
    {
        Jugador* jugadorActivo = getJugadorActivo();

        if (estadoHechizo_ != INACTIVO) // si hay un hechizo activo, se mueve el cursor
        {
            cursor.mover(dx, dy);
            return; // corta aquí para que no intente mover al granjero
        }

		if (!jugadorActivo->tienePiezaAgarrada()) // si no tiene pieza agarrada, se mueve el cursor
        {
            cursor.mover(dx, dy);
        }
		else // si tiene una pieza agarrada, se intenta mover la pieza
        {
            Animal* pieza = jugadorActivo->getPiezaSeleccionada();
            if (pieza->getEnMovimiento()) return;

            bool movimiento_valido = false;
            movimiento_valido = pieza->mover(TABLERO, dx, dy);
        }
    }
}

void Tablero::seleccionarPieza(int jugador, RenderizadorAudio* audio)
{
    if (turno_actual_ == jugador)
    {
        Cursor& cursor = getCursorActivo();
        Jugador* jugadorActivo = getJugadorActivo();
        Animal* casilla = casillas_[cursor.fila][cursor.columna];

        if (estadoHechizo_ != INACTIVO) {
            ejecutarPasoHechizo(casilla, cursor.fila, cursor.columna);
            return;
        }

        // CASO 1: LEVANTAR UNA PIEZA
        if (!jugadorActivo->tienePiezaAgarrada() && casilla != nullptr)
        {
            if (casilla->equipo_ == jugador)
            {
                if (casilla->atrapado_) {
                    std::cout << "[!] Este animal esta atrapado en la red. Le quedan " << casilla->ciclos_atrapado_ << " turnos.\n";
                    return; // bloquea recoger la pieza
                }

                // guardar el origen antes de levantarla físicamente
                casilla->casillaInicial_ = { cursor.fila, cursor.columna };

                jugadorActivo->agarrarPieza(casilla);
                casillas_[cursor.fila][cursor.columna] = nullptr;

                audio->sonarPickeo(casilla);
            }
        }
        // CASO 2: SOLTAR UNA PIEZA (Intentar movimiento)
        else if (jugadorActivo->tienePiezaAgarrada())
        {
            Animal* pieza = jugadorActivo->getPiezaSeleccionada();

            // si la pieza sigue desplazándose visualmente, evitar soltarla
            if (pieza->getEnMovimiento()) return;

            Movimiento m;
            m.origen = pieza->casillaInicial_;

            // leer la coordenada destino matemática a partir de los píxeles
            m.destino.columna = std::round((pieza->getPosX() - 152.0f) / 22.0f);
            m.destino.fila = 8 - std::round((pieza->getPosY() - 47.0f) / 22.0f);

            if (esMovimientoLegal(m))
            {
                if (getHayColision()) // asignar animales de combate a piezas chocantes (J1 izquierda, J2 derecha siempre)
                {
                    if (pieza->equipo_ == 0)
                    {
                        jugadores_[0]->setAnimalEnCombate(pieza);
                        jugadores_[1]->setAnimalEnCombate(casillas_[m.destino.fila][m.destino.columna]);
                    }
                    else
                    {
                        jugadores_[0]->setAnimalEnCombate(casillas_[m.destino.fila][m.destino.columna]);
                        jugadores_[1]->setAnimalEnCombate(pieza);
                    }

                    casillaDisputada = m.destino;
                    enBatalla = true;
                }

                mover(m);

                if (enBatalla)
                {
                    casillas_[m.destino.fila][m.destino.columna] = nullptr;
                }

                // teletransporta el cursor a la nueva casilla
                // aprovechando cursor.mover iterando hasta la meta con while
                while (cursor.columna < m.destino.columna) cursor.mover(1, 0);
                while (cursor.columna > m.destino.columna) cursor.mover(-1, 0);
                while (cursor.fila > m.destino.fila) cursor.mover(0, 1);  // dy=1 es ARRIBA (resta fila)
                while (cursor.fila < m.destino.fila) cursor.mover(0, -1);

                jugadorActivo->soltarPieza();

                // actualizar los animales atrapados tras un movimiento
                avanzarTurnosAtrapados();

                // cambio de turno
                turno_actual_ = (turno_actual_ == 0) ? 1 : 0;
                letreroTurnos_.setState(0, turno_actual_);                
            }
            else
            {
                // MOVIMIENTO ILEGAL: volver a colocar la pieza en su posición original
                casillas_[m.origen.fila][m.origen.columna] = pieza;

                float origX = 141.0f + 11.0f + (22.0f * m.origen.columna);
                float origY = 36.0f + 11.0f + (22.0f * (8 - m.origen.fila)); // lo de (8 - m.origen.fila) se explica más abajo en la linea 274
                pieza->setPosX(origX);
                pieza->setPosy(origY);

                // parar el movimiento de la pieza, por si acaso
                pieza->setVelX(0);
                pieza->setVelY(0);
                pieza->setEnMovimiento(false);
                pieza->avanzando_casilla_ = 0;
                pieza->casillas_movidas_ = 0;
                pieza->casillas_movidas_x_ = 0;
                pieza->casillas_movidas_y_ = 0;

                jugadorActivo->soltarPieza(); // soltar la pieza aunque el movimiento sea ilegal
                // no se cambia el turno, el jugador vuelve a intentarlo
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

        // calcula la casilla sobre la que está volando el animal
        int destCol = std::round((pieza->getPosX() - 152.0f) / 22.0f); // round sirve para redondear al entero más cercano
        int destFila = 8 - std::round((pieza->getPosY() - 47.0f) / 22.0f);

        // Si está dentro del tablero y hay un enemigo, activar colisión
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

bool Tablero::esMovimientoLegal(const Movimiento& m) const
{
    // validar que el destino esté dentro de las dimensiones del tablero
    if (m.destino.fila < 0 || m.destino.fila >= Constantes::FILAS_TABLERO ||
        m.destino.columna < 0 || m.destino.columna >= Constantes::COLUMNAS_TABLERO)
        return false;

    // obtener el jugador activo
    Jugador* jugadorActivo = jugadores_[turno_actual_];
    if (!jugadorActivo || !jugadorActivo->tienePiezaAgarrada())
        return false;

    // obtener la pieza seleccionada
    Animal* pieza = jugadorActivo->getPiezaSeleccionada();
    if (!pieza)
        return false;

    // comprobar colisión con piezas del propio equipo
    Animal* casillaDestino = casillas_[m.destino.fila][m.destino.columna];
    if (casillaDestino != nullptr && casillaDestino->equipo_ == turno_actual_)
        return false;

    // conexión con los vectores de movimientos posibles de cada animal
    std::vector<Movimiento> permitidos = pieza->movimientosPosibles();
    bool esValidoGeometricamente = false;

    for (const auto& mov : permitidos)
    {
        // comprobar si el destino propuesto coincide con uno de los destinos calculados por el animal
        if (mov.destino.fila == m.destino.fila && mov.destino.columna == m.destino.columna)
        {
            esValidoGeometricamente = true;
            break;
        }
    }

    return esValidoGeometricamente;
}

void Tablero::mover(const Movimiento& m)
{
    Jugador* jugadorActivo = getJugadorActivo();
    Animal* pieza = jugadorActivo->getPiezaSeleccionada();

    // mover la pieza en la matriz lógica del tablero
    casillas_[m.destino.fila][m.destino.columna] = pieza;

    // sincronizar la posición física/gráfica del animal con su nuevo destino
    float nuevaPosX = 141.0f + 11.0f + (22.0f * m.destino.columna);
    float nuevaPosY = 36.0f + 11.0f + (22.0f * (8 - m.destino.fila)); // invertir el eje Y para que la fila 0 esté en la parte inferior del tablero
    // esto es porque en la lógica del tablero, la fila 0 es la inferior, pero en el dibujo, la fila 0 está en la parte superior.
    // habría que cambiar alguna de las dos cosas para que no haya que hacer esta conversión, pero es un detalle menor y no afecta a la lógica del juego

    pieza->setPosicion(Vector2D(nuevaPosX, nuevaPosY)); // este tipo de uso de vector2D hay que hacerlo en todo el código

    // parar el movimiento de la pieza, por si acasoS
    pieza->setVelocidad(Vector2D(0, 0));

    pieza->setEnMovimiento(false);
    pieza->avanzando_casilla_ = 0;

    // limpiar los contadores de casillas movidas
    pieza->casillas_movidas_ = 0;
    pieza->casillas_movidas_x_ = 0;
    pieza->casillas_movidas_y_ = 0;
}

int Tablero::determinarGanador() {

    if (casillas_[4][4] != nullptr && casillas_[4][0] != nullptr && casillas_[4][8] != nullptr && casillas_[0][4] != nullptr && casillas_[8][4] != nullptr)
        if (casillas_[4][4]->equipo_ == casillas_[4][0]->equipo_ &&
            casillas_[4][4]->equipo_ == casillas_[4][8]->equipo_ &&
            casillas_[4][4]->equipo_ == casillas_[0][4]->equipo_ &&
            casillas_[4][4]->equipo_ == casillas_[8][4]->equipo_)
            return casillas_[4][4]->equipo_;

    // + condición de ganar por eliminación

    return -1;
}

void Tablero::acomodarGanador(Animal* animalGanador)
{
    casillas_[casillaDisputada.fila][casillaDisputada.columna] = animalGanador;
    casillas_[casillaDisputada.fila][casillaDisputada.columna]->
    setPosicion({ 141.0f + 11.0f + 22.0f * casillaDisputada.columna, 36.0f + 11.0f + 22.0f * (8 - casillaDisputada.fila) });
}