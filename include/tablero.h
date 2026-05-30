#pragma once
#include "animal.h"  
#include "cursor.h" 
#include "tarjetaID.h" 
#include "jugador.h"
#include "estructuras.h"
#include "RenderizadorAudio.h"
const int BANDO_LUZ = 0;
const int BANDO_OSCURIDAD = 1;
const int CASILLA_LUZ = 0;
const int CASILLA_OSCURA = 1;

enum EstadoHechizo { // para controlar el estado de los hechizos
    INACTIVO,
    TELETRANSPORTE_SELECCIONAR_ALIADO,
    TELETRANSPORTE_SELECCIONAR_DESTINO,
    CURAR_SELECCIONAR_ALIADO,
    INTERCAMBIO_SELECCIONAR_ALIADO,
    INTERCAMBIO_SELECCIONAR_ENEMIGO,
    ATRAPAR_SELECCIONAR_ENEMIGO
};

struct Letrero 
{
    Vector2D posicion = { 550.0f, 65.0f };

    int frameActualX_ = 0, frameActualY_ = 0;
    float timer{}, msStep = 65;
    int nFrames = 8;
    void animar(float dt);
    bool loop = false;
    void setState(int frameX, int frameY);
};

struct Pato 
{
    Vector2D posicion = { 60, 90 };
    bool subiendo = true;
    
    int frameActualX_ = 0, frameActualY_ = 0;
    float timer{}, msStep = 100;
    int nFrames = 4;
    void animar(float dt);
    bool loop = true;
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

    static const int TAMANO_CASILLA = 22;
    static const int X_INICIO = 141;
    static const int Y_INICIO = 36;
  // devuelve el cursor del jugador con el turno

public:

    float angulo = 0;
    Pato pato;

    Casilla casillaDisputada{};
    bool enBatalla = false;
    
    Animal* animalesEnBatalla[2]{};

    Tablero(Jugador* jugador1, Jugador* jugador2);
    ~Tablero();
    Tablero(const Tablero&) = delete; // para evitar que se pueda copiar el tablero
    Tablero& operator=(const Tablero&) = delete; // no se puede asignar un tablero a otro

    void inicializarTablero();
    void recibirMovimiento(int jugador, int dx, int dy);
    void seleccionarPieza(int jugador, RenderizadorAudio* audio);
    void actualizar(float dt);
    void actualizarColision();
    int determinarGanador();

    bool getHayColision() const { return hay_colision_; }
    const Cursor& getCursorActivo() const { return turno_actual_ == 0 ? cursorJ1_ : cursorJ2_; }
    Cursor& getCursorActivo() { return turno_actual_ == 0 ? cursorJ1_ : cursorJ2_; }
    const Jugador* getJugadorActivo() const { return jugadores_[turno_actual_]; }
	Jugador* getJugadorActivo() { return jugadores_[turno_actual_]; }
	float getLetreroPosX() const { return letreroTurnos_.posicion.x; }
	float getLetreroPosY() const { return letreroTurnos_.posicion.y; }
	int getLetreroFrameX() const { return letreroTurnos_.frameActualX_; }
	int getLetreroFrameY() const { return letreroTurnos_.frameActualY_; }
	void setLetreroPosX(float x) { letreroTurnos_.posicion.x = x; }
	void setLetreroPosY(float y) { letreroTurnos_.posicion.y = y; }

	Animal* getAnimalEnCasilla(int fila, int columna) const { return casillas_[fila][columna]; }
    void acomodarGanador(Animal* animalGanador);

	const Tarjeta* getTarjeta() const { return &tarjeta; }

    Tarjeta tarjeta;

    // propuesta de psanse en la tutoria:
    bool esMovimientoLegal(const Movimiento& m) const;
    void mover(const Movimiento& m);
    //bool hayColisionEnemiga(const Movimiento& m) const;

	// hechizos
    EstadoHechizo estadoHechizo_ = INACTIVO;
    int teclaHechizoActivo_ = -1;
    Animal* primerObjetivoHechizo_ = nullptr;
    bool hechizoDisponible_[2][5]; // matriz para controlar si un jugador tiene disponible cada hechizo

    EstadoHechizo getEstadoHechizo() const { return estadoHechizo_; }

    void procesarTeclaHechizo(int tecla);
    void ejecutarPasoHechizo(Animal* casilla, int fila, int col);
    void finalizarHechizo();
    void avanzarTurnosAtrapados();
};