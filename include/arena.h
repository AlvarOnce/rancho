#pragma once
#include "animal.h"
#include "renderizador.h"	

//dimensiones que ocupa la arena en la pantalla

const int ANCHO_DE_LA_ARENA = 480;
const int ALTO_DE_LA_ARENA = 270;

//dimensiones de la zona de combate
const int ZONA_DE_COMBATE_X = 198;
const int ZONA_DE_COMBATE_Y = 198;

//calculo de margenes de la arena
const float ARENA_MARGEN_X = (ANCHO_DE_LA_ARENA - ZONA_DE_COMBATE_X) / 2.0f;
const float ARENA_MARGEN_Y = (ALTO_DE_LA_ARENA - ZONA_DE_COMBATE_Y) / 2.0f;

//limites para los moves y ataques
const float LIMITE_IZQ_ARENA = ARENA_MARGEN_X;
const float LIMITE_DCH_ARENA = ARENA_MARGEN_X + ZONA_DE_COMBATE_X;
const float LIMITE_ARRIBA_ARENA = ARENA_MARGEN_Y;
const float LIMITE_ABAJO_ARENA = ARENA_MARGEN_Y + ZONA_DE_COMBATE_Y;

const int NUM_DE_BARRERAS = 8;

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
	float pos_antigua_x[2];
	float pos_antigua_y[2];
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
	bool barrera_visible[NUM_DE_BARRERAS];
	float contador_ciclo_barrera[8];
	float ciclo_maximo_barrera[8];
	int ganador;
	bool combate_terminado;

	//atributos de ataque a melee
	bool ataque_activo[2];
	float ataque_x[2];
	float ataque_y[2];
	float ataque_visible_tiempo[2];
	static const float DURACION_ATAQUE;

	void actualizarMovimiento(float dt);
	void actualizarDisparo(float dt);
	void actualizarRecarga(float dt);
	void actualizarBarreras(float dt);
	void confirmarImpacto();
	void confirmarFinCombate();
	void mantenerLimites(int jugador);
	bool colisionBarrera(float x, float y);
	void colocarBarrerasAleatorias();
	void actualizarAtaque(float dt);
	
public:
	Arena();
	~Arena();

	void inicioCombate(Animal* pieza_luz, Animal* pieza_oscuridad);
	void actualizar(float dt);
	void dibujar(Renderizador* renderizador);
	void recibirMovimiento(int jugador, int movimiento, bool tecla_pulsada);
	bool recibirAtaque(int jugador);
	int obtenerPerdedor() const; 
	bool combateTerminado() const;
	int ganadorCombate() const;
};

