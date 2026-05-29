#pragma once
#include <iostream>
#include <vector>
#include "ETSIDI.h"
#include "estructuras.h"

enum modoJuego 
{
	TABLERO, BATALLA, CANCELAR
};

enum tipoAnimacion 
{
	QUIETO, CAMINAR, ATACAR,
};

enum especieAnimal 
{
	CABRA, CERDO, GALLINA, OVEJA, GRANJERO
};

class Animal
{
public:

	Animal(float posx, float posy, float capa, int vida, float xinicial, int equipo)
		: posicion_(posx, posy), capaz_(capa), vida_(vida), xinicial_(xinicial), equipo_(equipo) {

		if (equipo_ == 0)
			setState(0, 0);
		else if (equipo_ == 1)
			setState(0, 1);

	}

	virtual ~Animal() {}

	// Lógica
	Vector2D posicion_;
	Vector2D velocidad_;

	float capaz_;
	int equipo_;
	int vida_;
	int ataque_{ 0 };
	float avanzando_casilla_ = 0;	// para saber cuando ha terminado de moverse
	bool en_movimiento_ = false;	// para bloquear el teclado si se esta moviendo
	int casillas_movidas_x_ = 0;
	int casillas_movidas_y_ = 0;
	int casillas_movidas_ = 0;
	int max_casillas_movidas_ = 100; // esto se puede modificar en cada clase hija, para que cada animal tenga un número de casillas a mover diferente
	bool intro_tablero_ = true;
	float xinicial_ = 152;

	int casillaInicial_[2] = { 0,0 };

	especieAnimal especie_;
	
	bool mover(modoJuego modo, int dx, int dy);	//ahora es un bool, si devuelve true se ha movido bien,
												// si devuelve false, no se ha movido
	virtual void atacar()						
	{
		std::cout << "Soy un animal genérico, mi ataque no está definido.";
	}

	float getPosX() const { return posicion_.x; }
	float getPosY() const { return posicion_.y; }
	float getVelX() const { return velocidad_.x; }
	float getVelY() const { return velocidad_.y; }
	float getCapaz() const { return capaz_; }
	bool getEnMovimiento() const { return en_movimiento_; }
	int getMaxCasillasMovidas() const { return max_casillas_movidas_; } //para sacarlo de la clase hija
	bool getIntroTablero() const { return intro_tablero_; }

	int getFrameActualX() const { return frameActualX_; }
	int getFrameActualY() const { return frameActualY_; }

	especieAnimal getEspecie() const { return especie_; }
	bool getVivo() const { return vida_ > 0; }
	int getEquipo() const { return equipo_; }

//protected: // Solo los hijos animales pueden modificar sus posiciones

	void setPosicion(const Vector2D& pos) { posicion_ = pos; }
	void setPosX(float posx) { posicion_.x  = posx; }
	void setPosy(float posy) { posicion_.y  = posy; }
	void setVelocidad(const Vector2D& vel) { velocidad_ = vel; }
	void setVelX(float velx) { velocidad_.x = velx; } 
	void setVelY(float vely) { velocidad_.y = vely; }
	void setEnMovimiento(bool en_movimiento) { en_movimiento_ = en_movimiento; }

	// Dibujo y animación
	int frameActualX_ = 0;
	int frameActualY_ = 1;
	int nFrames = 1;
	float timer = 0;
	float msStep = 100;
	bool pausa = true;
	int ancho = 0;
	int alto = 0;
	void setState(int frameX, int frameY);
	void animar(float dt);

	virtual void actualizar(float dt);

	// funcion virtual
	virtual std::vector<Movimiento> movimientosPosibles() const; 
};