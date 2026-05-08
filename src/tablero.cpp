#include "tablero.h"

Tablero::Tablero(Animal** animalesJ1, Animal** animalesJ2)
{
    inicializarTablero();

    for (int j = 0; j < 2; j++)
    {
        for (int i = 0; i < FILAS; i++)
        {
            casillas[i][j] = animalesJ1[j * FILAS + i];
        }
    }

    for (int j = 0; j < 2; j++)
    {
        for (int i = 0; i < FILAS; i++)
        {
            casillas[i][8-j] = animalesJ2[j * FILAS + i];
        }
    }
}

Tablero::~Tablero()
{

}

void Tablero::inicializarTablero() // iniclizaiamos el Tablero vacio, es decir, creamos la matriz pero no le decimos todavia si hay figuras o no en las casillas
{
    for (int i = 0; i < FILAS; i++)
    {
        for (int j = 0; j < COLUMNAS; j++)
        {
            casillas[i][j] = nullptr;

            if ((i + j) % 2 == 0)
            {
                color_casilla[i][j] = CASILLA_LUZ; // como ya estan creadas las celdas de la matriz, ahora le decimos a cada celda que es, si es luz o oscuradad
            }
            else
            {
                color_casilla[i][j] = CASILLA_OSCURA;
            }
        }
    }


    turno_actual = BANDO_LUZ; // incia el turno el bando de luz


}

void Tablero::actualizar(float dt)
{
    // actualiza todos los animales posados sobre el tablero
    for (int i = 0; i < FILAS; i++)
    {
        for (int j = 0; j < COLUMNAS; j++)
        {
            if (casillas[i][j] != nullptr)
            {
                casillas[i][j]->actualizar(dt);
            }
        }
    }

    // actualiza el animal levantado en el cursor
    if (animal_seleccionado_ != nullptr)
    {
        animal_seleccionado_->actualizar(dt);
    }

        letreroTurnos.posx = 102 + turno_actual * 273;
        letreroTurnos.animar(dt);
}

void Tablero::dibujar(Renderizador* motor){

    // imagen de fondo del tablero
    motor->dibujarSprite("../assets/Sprites/tablero/tableroFondo.png", 512, 512, 480/2, 270/2, -1);
    motor->dibujarSprite("../assets/Sprites/tablero/tablero.png", 256, 256, 480 / 2, 270 / 2, -2);
    motor->dibujarSprite("../assets/Sprites/tablero/turnos.png", 256, 128, letreroTurnos.posx, 270 / 2, -5, 4, 8, letreroTurnos.frameActualX_, letreroTurnos.frameActualY_);

    // dibuja los animales posados sobre el tablero
    for (int i = 0; i < FILAS; i++)
    {
        for (int j = 0; j < COLUMNAS; j++)
        {
            if (casillas[i][j] != nullptr)
            {
                casillas[i][j]->dibujar(motor);
            }
        }
    }

    // dibuja el animal levantado en el cursor
    if (animal_seleccionado_ != nullptr)
    {
        animal_seleccionado_->dibujar(motor);
    }

	cursor.dibujar(motor);

    // if(el cursor esta sobre un animal) funcion propia de tablero y cursor detectar la casilla
        // en actualizar se determina que tarjeta se va a dibujar interiormente
    if (cursor.posx > 150 && cursor.posx < 170) // ELIMINAR ESTA CONDICION
        tarjeta.dibujar(motor);
}



void Tablero::recibirMovimiento(int jugador, int dx, int dy)
{
    // La pieza [8][0] es la última en llegar
	if (casillas[8][0] != nullptr && casillas[8][0]->getIntroTablero()) return; // si no ha terminado su animación de introducción, bloqueamos el movimiento del tablero
   
    if (jugador == turno_actual) // El tablero decide si el movimiento recibido es ejecutable según el turno, por ser su dueño
    { 
        if (hay_pieza_seleccionada_ == FALSE)
        {                                               
             cursor.mover(dx, dy);
        }
        else // tengo una pieza
        {
            if (animal_seleccionado_->getEnMovimiento()) return; // si el animal seleccionado se está moviendo, bloqueamos el movimiento del tablero, esto impide jugar rápido
            bool movimiento_valido = false;
            if (dx == 0 && dy == 1)   movimiento_valido = animal_seleccionado_->mover(TABLERO, U);
            if (dx == 0 && dy == -1)   movimiento_valido = animal_seleccionado_->mover(TABLERO, D);
            if (dx == -1 && dy == 0)   movimiento_valido = animal_seleccionado_->mover(TABLERO, L);
            if (dx == 1 && dy == 0)   movimiento_valido = animal_seleccionado_->mover(TABLERO, R);

            if (movimiento_valido) {
                cursor.mover(dx, dy);
            }
        }

        std::cout << cursor.fila << " " << cursor.columna << std::endl;
        //std::cout << casillas[cursor.columna][cursor.fila] << std::endl; // esto es para ver si el cursor se mueve por la matriz, si el puntero es null o no
    }
}

void Tablero::seleccionarPieza(int jugador) // falta que tenga en cuenta el turno, que solo se pueda seleccionar una pieza del bando del jugador que le toca
{                                           // seguramente con la clase Jugador sea más fácil

    if (turno_actual == jugador) // solo puede seleccionar la pieza aquel jugador con el turno, y solo las suyas
    {
        if (casillas[cursor.fila][cursor.columna] == nullptr && animal_seleccionado_ == nullptr) {

        }
        else if (casillas[cursor.fila][cursor.columna] != nullptr && animal_seleccionado_ == nullptr) {

            if (casillas[cursor.fila][cursor.columna]->equipo_ == jugador) // para poder solo seleccionar mis piezas
            {
                hay_pieza_seleccionada_ = !hay_pieza_seleccionada_;
                animal_seleccionado_ = casillas[cursor.fila][cursor.columna];
                casillas[cursor.fila][cursor.columna] = nullptr;
            }
        }
        else if (casillas[cursor.fila][cursor.columna] == nullptr && animal_seleccionado_ != nullptr) {
            casillas[cursor.fila][cursor.columna] = animal_seleccionado_;
            hay_pieza_seleccionada_ = !hay_pieza_seleccionada_;
            animal_seleccionado_ = nullptr;

            if (turno_actual == 0)
                turno_actual = 1;
            else
                turno_actual = 0;

            letreroTurnos.setState(0, turno_actual);
        }
        else if (casillas[cursor.fila][cursor.columna] != nullptr && animal_seleccionado_ != nullptr)
        {

        }
    }
}

void Letrero::animar(float dt) {

    timer = timer + dt;

    if (timer > msStep)
    {
        if (frameActualX_ < nFrames - 1) frameActualX_++;
        else if (loop) frameActualX_ = 0;

        timer = timer - msStep;
    }
}

void Letrero::setState(int frameX, int frameY) {

    frameActualX_ = frameX;
    frameActualY_ = frameY;
    loop = false;

}