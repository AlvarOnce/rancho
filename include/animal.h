#pragma once
#include <iostream>
#include <vector>
#include "ETSIDI.h"
#include "estructuras.h"
#include "ataque.h"

enum modoJuego 
{
	TABLERO, BATALLA, CANCELAR
};

enum especieAnimal
{
	CABRA, CERDO, GALLINA, GRANJERO, LLAMA, OVEJA
};

class Animal
{
protected:
	// Lógica
	Vector2D posicion_;
	Vector2D velocidad_;
	especieAnimal especie_;

	float capaz_{};
	int equipo_;
	int vida_; // vida que cambia durante el combate
	int vida_base_; // vida base del animal
	int bonus_vida_casilla_ = 0; // bonus de vida temporal por estar en casilla de su color
	bool vivo_ = true;

public:

	Animal(Casilla casillaInicial, int equipo) : casillaInicial_(casillaInicial), equipo_(equipo) {

		if (equipo_ == 0)
		{
			posicion_ = { -44.0f - 15.0f * casillaInicial_.fila + 11.0f + 44.0f * casillaInicial_.columna, 36.0f + 176.0f - (22.0f * casillaInicial_.fila) + 11.0f };
			capaz_ = -3.0f + 0.01f * casillaInicial_.fila + 0.01f * casillaInicial_.columna;
			setState(0, 0);
		}

		if (equipo_ == 1)
		{
			posicion_ = { 480.0f + 15.0f * casillaInicial_.fila - 11.0f + 44.0f * (casillaInicial_.columna - 7), 36.0f + 176.0f - (22.0f * casillaInicial_.fila) + 11.0f };
			capaz_ = -3.5f + 0.01f * casillaInicial_.fila + 0.01f * casillaInicial_.columna;
			//480.0f + 44.0f + 15.0f * casillaInicial_.fila - 11.0f 
			setState(0, 1);
		}
		ataque_ = nullptr;
	}

	virtual ~Animal() {}

	Casilla casillaInicial_{};

	Ataque* ataque_;
	float avanzando_casilla_ = 0;	// para saber cuando ha terminado de moverse
	bool en_movimiento_ = false;	// para bloquear el teclado si se esta moviendo
	int casillas_movidas_x_ = 0;
	int casillas_movidas_y_ = 0;
	int casillas_movidas_ = 0;
	int max_casillas_movidas_ = 100; // esto se puede modificar en cada clase hija, para que cada animal tenga un número de casillas a mover diferente
	bool intro_tablero_ = true;
	float xinicial_ = 152;

	bool atrapado_ = false;
	int ciclos_atrapado_ = 0;

	bool mover(modoJuego modo, int dx, int dy);	//ahora es un bool, si devuelve true se ha movido bien,
												// si devuelve false, no se ha movido
	virtual void atacar()						
	{
		std::cout << "Soy un animal genérico, mi ataque no está definido.";
	}

	float getVida() const { return vida_; }
	float getEquipo() const { return equipo_; }
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
	bool getVivo() const { return vivo_; }

//protected: // Solo los hijos animales pueden modificar sus posiciones

	void setVida(const int vida) { vida_ = vida; }
	void setVivo(const bool vivo) { vivo_ = vivo; }
	void setPosicion(const Vector2D& pos) { posicion_ = pos; }
	void setPosX(float posx) { posicion_.x  = posx; }
	void setPosy(float posy) { posicion_.y  = posy; }
	void setCapaz(float capaz) { capaz_ = capaz; }
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

	virtual void actualizarEnTablero(float dt);
	virtual void actualizarEnBatalla(float dt);

	// funcion virtual
	virtual std::vector<Movimiento> movimientosPosibles() const; 
	// ATAQUE
	Ataque* getAtaque() const { return ataque_; }
	int   getDanoAtaque()    const { return ataque_ ? ataque_->getDano() : 0; }
	float getAlcanceAtaque() const { return ataque_ ? ataque_->getAlcance() : 0.f; }
	float getRecargaAtaque() const { return ataque_ ? ataque_->getRecarga() : 0.f; }
	float getRecargaActual() const { return ataque_->getRecarga(); }
	virtual const char* getTipoAtaque() const { return "No estoy definido"; };
	void recibirDano(int dano) { vida_ -= dano; }

	// casillas de colores
	void setBonusVidaCasilla(int bonus) { bonus_vida_casilla_ = bonus; }
	int getBonusVidaCasilla() const { return bonus_vida_casilla_; }
	int getVidaBase() const { return vida_base_; }
	void setVidaBase(int v) { vida_base_ = v; }
};