#include "tablero.h"

Tablero::Tablero(Animal** misAnimales)
{
    inicializarTablero();

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < FILAS; j++)
        {
            casillas[1-i][j] = misAnimales[j+(i*FILAS)];
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
}

void Tablero::dibujar(Renderizador* motor){

    // imagen de fondo del tablero
    motor->dibujarSprite("../assets/Sprites/tablero/tableroFondo.png", 512, 512, 480/2, 270/2, -1);
    motor->dibujarSprite("../assets/Sprites/tablero/tablero.png", 256, 256, 480 / 2, 270 / 2, -2);

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

	// dibuja el cursor
	cursor.dibujar(motor);

    // if(el cursor esta sobre un animal) funcion propia de tablero y cursor detectar la casilla
        // en actualizar se determina que tarjeta se va a dibujar interiormente
    if (cursor.posx > 150 && cursor.posx < 170) // ELIMINAR ESTA CONDICION
        tarjeta.dibujar(motor);
}

//la 'd' y la flecha izquierda las dos glut las lee como 100
//std::cout << "posicion del cursor: " << cursor.columna << ", " << cursor.fila << std::endl;
//std::cout << casillas[cursor.columna][cursor.fila] << std::endl; // esto es para ver si el cursor se mueve por la matriz, si el puntero es null o no

void Tablero::recibirMovimiento(int jugador, int dx, int dy)
{
	if (casillas[0][0] != nullptr && casillas[0][0]->getIntroTablero()) return; // si no ha terminado su animación de introducción, bloqueamos el movimiento del tablero

    if (hay_pieza_seleccionada_ == FALSE)
    {                                               // queda mucho más compacto así, pasando directamente los parámetros al método mover del cursor
		if (turno_actual == BANDO_LUZ)              // sin necesidad de un montón de ifs anidados
			if (jugador == 0) cursor.mover(dx, dy); // si el turno es del bando de luz, solo se puede mover el cursor del jugador 1 (el de la izquierda)
		if (turno_actual == BANDO_OSCURIDAD)        // habrá que modificar esto cuando haya un cursor para cada jugador.
			if (jugador == 1) cursor.mover(dx, dy);
    }
    else 
    {
		if (animal_seleccionado_->getEnMovimiento()) return; // si el animal seleccionado se está moviendo, bloqueamos el movimiento del tablero
		                                                     // esto impide jugar rápido
		bool movimiento_valido = false;
        if (dx == 0 && dy == 1)   movimiento_valido = animal_seleccionado_->mover(TABLERO, U);
        if (dx == 0 && dy == -1)   movimiento_valido = animal_seleccionado_->mover(TABLERO, D);
        if (dx == -1 && dy == 0)   movimiento_valido = animal_seleccionado_->mover(TABLERO, L);
        if (dx == 1 && dy == 0)   movimiento_valido = animal_seleccionado_->mover(TABLERO, R);

        if (movimiento_valido) {
            cursor.mover(dx, dy); 
        }
    }   
}

void Tablero::seleccionarPieza(int jugador) // falta que tenga en cuenta el turno, que solo se pueda seleccionar una pieza del bando del jugador que le toca
{                                           // seguramente con la clase Jugador sea más fácil
    if (casillas[cursor.columna][cursor.fila] == nullptr && animal_seleccionado_ == nullptr) {

    }
    else if (casillas[cursor.columna][cursor.fila] != nullptr && animal_seleccionado_ == nullptr) {
        hay_pieza_seleccionada_ = !hay_pieza_seleccionada_;
        animal_seleccionado_ = casillas[cursor.columna][cursor.fila];
        casillas[cursor.columna][cursor.fila] = nullptr;
    }
    else if (casillas[cursor.columna][cursor.fila] == nullptr && animal_seleccionado_ != nullptr) {
        casillas[cursor.columna][cursor.fila] = animal_seleccionado_;
        hay_pieza_seleccionada_ = !hay_pieza_seleccionada_;
		animal_seleccionado_ = nullptr;
    }
    else if (casillas[cursor.columna][cursor.fila] != nullptr && animal_seleccionado_ != nullptr)
    {

    }
}
