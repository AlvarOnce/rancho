#pragma once
#include "animal.h"  
#include "cursor.h" 
#include "tarjetaID.h" 
#include "jugador.h"

const int BANDO_LUZ = 0;
const int BANDO_OSCURIDAD = 1;
const int CASILLA_LUZ = 0;
const int CASILLA_OSCURA = 1;

struct Letrero 
{
    float posx = 550;
    float posy = 65;
    int frameActualX_ = 0, frameActualY_ = 0;
    float timer{}, msStep = 65;
    int nFrames = 8;
    void animar(float dt);
    bool loop = false;
    void setState(int frameX, int frameY);
};

class Tablero 
{
    Animal* casillas_[Constantes::FILAS_TABLERO][Constantes::COLUMNAS_TABLERO];
    int color_casilla_[Constantes::FILAS_TABLERO][Constantes::COLUMNAS_TABLERO];
    bool hay_colision_ = false;
    int turno_actual_ = BANDO_LUZ;
    Letrero letreroTurnos_;

    Jugador* jugadores_[2];// que recuerda donde estaba al cambiar el turno// para poder acceder a su pieza seleccionada
    Cursor cursorJ1_ = Cursor(2,4,0); // se inicia cursor en columna,fila
    Cursor cursorJ2_ = Cursor(6,4,1);
    Cursor& getCursorActivo();// devuelve el cursor del jugador con el turno

    static const int TAMANO_CASILLA = 22;
    static const int X_INICIO = 141;
    static const int Y_INICIO = 36;
  // devuelve el cursor del jugador con el turno

public:

    bool enBatalla = false;
    Animal* animalesEnBatalla[2]{};

    Tablero(Jugador* jugador1, Jugador* jugador2);
    ~Tablero();
    Tablero(const Tablero&) = delete; // para evitar que se pueda copiar el tablero
    Tablero& operator=(const Tablero&) = delete; // no se puede asignar un tablero a otro

    void inicializarTablero();
    void recibirMovimiento(int jugador, int dx, int dy);
    void seleccionarPieza(int jugador);
    void actualizar(float dt);
    void dibujar(Renderizador* motor);
    void actualizarColision();
    bool getHayColision() const { return hay_colision_; }
    Jugador* getJugadorActivo();

    Tarjeta tarjeta;

    // propuesta de psanse en la tutoria:
    bool esMovimientoLegal(const Movimiento& m) const;
    void mover(const Movimiento& m);
    //bool hayColisionEnemiga(const Movimiento& m) const;
};