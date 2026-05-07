#pragma once
#include "animal.h"
#include "renderizador.h"	

const int ANCHO_DE_LA_ARENA = 480;
const int ALTO_DE_LA_ARENA = 270;
const int NUM_DE_BARRERAS = 6;

const int ARRIBA = 0;
const int ABAJO = 1;
const int IZQUIERDA = 2;
const int DERECHA = 3;

const int ATAQUE_TIPO_DISPARO = 1;
const int ATAQUE_TIPO_GOLPE = 2;
const int ATAQUE_EN_AREA = 3;

class Arena
{
	Animal* combatientes[2]; // si quereis Bnado Luz [0], [1] para el otro bando.
	float pos_x[2];
	float pos_y[2];
	float ultima_direccion_x[2];
	float ultima_direccion_y[2];
	bool vivo[2];

	bool movimiento_arriba[2];
	bool movimiento_abajo[2];
	bool movimiento_izq[2];
	bool movimiento_dch[2];

	float disparo_x[2];
	float disparo_y[2];
	float direccion_disparo_x[2];
	float direccion_disparo_y[2];
	float disparo_disparado[2];// xd el nombre.

	float recarga_de_ataque[2];//tiempo que queda para poder atacar 
	float recarga_maxima_de_ataque[2];//tiempo de racarga total de cada pieza

	float barrera_x[NUM_DE_BARRERAS];
	float barrera_y[NUM_DE_BARRERAS];
	float barrera_visible[NUM_DE_BARRERAS];
	float contador_de_ciclo_barreras;
	float ciclo_maximo_barreras;

	int ganador;
	bool combate_terminado;

	void actualizarMovimiento(float dt);
	void actualizarDisparo(float dt);
	void actualizarRecarga(float dt);
	void actualizarBarreras(float dt);
	void confirmarImpacto();
	void confirmarFinCombate();
	void mantenerLimites(int jugador);
	bool colisionBarrera(float x, float y);
	void colocarBarrerasAleatorias();
	
public:
	Arena();
	~Arena();

	void inicioCombate(Animal* pieza_luz, Animal* pieza_oscuridad);
	void actualizar(float dt);
	void dibujar(Renderizador* renderizador);
	void recibirMovimiento(int jugador, int movimiento, bool tecla_pulsada);
	bool recibirAtaque(int jugador);

	bool combateTerminado() const;
	int ganadorCombate() const;
};

