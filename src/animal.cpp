#include "animal.h"

void Animal::actualizarEnTablero(float dt) 
{

    if (intro_tablero_) {

        if (equipo_ == 0)
        if (posicion_.x < 141 + 11 + 22 * casillaInicial_.columna) {
            posicion_.x += dt/25;
            animar(dt);
        }
        else {
            posicion_.x = 141 + 11 + 22 * casillaInicial_.columna; // Aseguramos que quede clavado en la posición exacta
            intro_tablero_ = false; 
            setState(0, 0);     
        }

        if (equipo_ == 1)
        if (posicion_.x > 141 + 11 + 22 * casillaInicial_.columna) {
            posicion_.x -= dt / 25;
            animar(dt);
        }
        else {
            posicion_.x = 141 + 11 + 22 * casillaInicial_.columna; // Aseguramos que quede clavado en la posición exacta
            intro_tablero_ = false;
            setState(0, 1);
        }

        return; // Salimos de la función para que no haga el movimiento del Tablero todavía
    } 

    if (!en_movimiento_) 
    { 
		setState(0, equipo_);
        return;
    }

    // movimiento usando el Vector 2D
    posicion_ += velocidad_ * (dt / 25);	    

    avanzando_casilla_ += velocidad_.modulo() * (dt / 25);

    if (avanzando_casilla_ >= 22) { // si el animal se ha movido más de una casilla, se para
        velocidad_.x = 0;
        velocidad_.y = 0;
        avanzando_casilla_ = 0;
		en_movimiento_ = false;
	}

    //cuando te vas a una casilla fuera del cuadrado, el juego mueve otra vez automaticamente a la casilla de antes
	if (posicion_.x <= 130) int trash = mover(TABLERO, 1, 0); // Limite izquierdo del tablero
	if (posicion_.x >= 130 + 22 * 10) int trash = mover(TABLERO, -1, 0); // Limite derecho del tablero
	if (posicion_.y <= 25) int trash = mover(TABLERO, 0, 1); // Limite inferior del tablero
	if (posicion_.y >= 25 + 22 * 10) int trash = mover(TABLERO, 0, -1); // Limite superior del tablero

    animar(dt);
}

void Animal::actualizarEnBatalla(float dt)
{
    // Aqui se debería estar el control de movimiento de animal, ahora en batalla
}

bool Animal::mover(modoJuego modo, int dx, int dy) // Para que el animal sepa que tipo de movimiento debe realizar, 
{   // se le debe indicar en que modo estamos
    // Agregar dt en los cambios de posición

    switch (modo) {

    case TABLERO: // Se llama desde tablero con animal.mover(TABLERO, direccion)

        // orientación de caminata: Esto funciona bien, porque mover se llama una vez entonces el estado solo se setea al principio, como esta definido en batalla no funciona
        if (dx == 1) setState(0, 0);
        if (dx == -1) setState(0, 1);
        if (dy == 1) setState(0, 2);
        if (dy == -1) setState(0, 3);

       	en_movimiento_ = true;
        casillas_movidas_x_ += dx;
        casillas_movidas_y_ += dy;

        velocidad_.x = dx;
        velocidad_.y = dy;
        if (velocidad_.x != 0 && velocidad_.y != 0) {
            velocidad_.x *= 0.7071f;
            velocidad_.y *= 0.7071f;
		}       
		
        casillas_movidas_ = abs(casillas_movidas_x_) + abs(casillas_movidas_y_);
        if (casillas_movidas_ > getMaxCasillasMovidas()) return mover(CANCELAR, dx, dy);
		else return true;

    case BATALLA: // Se llama desde batalla con animal.mover(BATALLA, direccion)



        return false;

	case CANCELAR: 
        velocidad_.x = 0;
        velocidad_.y = 0;
		casillas_movidas_x_ -= dx;
		casillas_movidas_y_ -= dy;
        en_movimiento_ = false;

        return false;
    }
	return false;

}

void Animal::animar(float dt)
{
        timer = timer + dt;
        if (timer > msStep)
        {
            if (frameActualX_ < nFrames-1) frameActualX_++;
            else frameActualX_ = 0;
            timer = timer - msStep;
        }
}

void Animal::setState(int frameX, int frameY) 
{
    frameActualX_ = frameX;
    frameActualY_ = frameY;
    pausa = true;

}

std::vector<Movimiento> Animal::movimientosPosibles() const
{
    std::vector<Movimiento> movimientos;
    //Casilla origen = { casillaInicial_[0], casillaInicial_[1] };
    Casilla origen = casillaInicial_;

    int alcance = max_casillas_movidas_;

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