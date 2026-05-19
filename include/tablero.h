#pragma once
#include "animal.h"  
#include "cursor.h" 
#include "tarjetaID.h" 
#include "jugador.h"

const int BANDO_LUZ = 0;
const int BANDO_OSCURIDAD = 1;
const int CASILLA_LUZ = 0;
const int CASILLA_OSCURA = 1;

struct Letrero {
    float posx = 550;
    float posy = 65;
    int frameActualX_ = 0, frameActualY_ = 0;
    float timer{}, msStep = 65;
    int nFrames = 8;
    void animar(float dt);
    bool loop = false;
    void setState(int frameX, int frameY);
};

class Tablero {

    Animal* casillas[9][9];
    int color_casilla[9][9];
    bool hay_colision_ = false;
    int turno_actual;
    Letrero letreroTurnos;

    Jugador* jugador1_;         // guardamos punteros a los jugadores
    Jugador* jugador2_; 
    Jugador* getJugadorActivo();// que recuerda donde estaba al cambiar el turno// para poder acceder a su pieza seleccionada
    Cursor cursorJ1_ = Cursor(141.0f + 11.0f, 36.0f + 11.0f + 22.0f * 8.0f, 0);// esquina inferior izquierda
    Cursor cursorJ2_ = Cursor(141.0f + 11.0f + 22.0f * 8.0f, 36.0f + 11.0f, 1);// esquina superior derecha 
    Cursor& getCursorActivo();// devuelve el cursor del jugador con el turno


    static const int TAMANO_CASILLA = 22;
    static const int FILAS = 9;
    static const int COLUMNAS = 9;
    static const int X_INICIO = 141;
    static const int Y_INICIO = 36;
  // devuelve el cursor del jugador con el turno

public:

    Tablero(Jugador* jugador1, Jugador* jugador2);
    ~Tablero();

    void inicializarTablero();
    void recibirMovimiento(int jugador, int dx, int dy);
    void seleccionarPieza(int jugador);
    void actualizar(float dt);
    void dibujar(Renderizador* motor);
    void actualizarColision();
    bool getHayColision() const { return hay_colision_; }

    Tarjeta tarjeta;
};