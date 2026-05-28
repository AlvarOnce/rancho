#pragma once
#include "animal.h"
// interaccion.h va en arena.cpp, no aquí

const int ANCHO_DE_LA_ARENA = 480;
const int ALTO_DE_LA_ARENA = 270;
const int ZONA_DE_COMBATE_X = 198;
const int ZONA_DE_COMBATE_Y = 198;

const float ARENA_MARGEN_X = (ANCHO_DE_LA_ARENA - ZONA_DE_COMBATE_X) / 2.0f;
const float ARENA_MARGEN_Y = (ALTO_DE_LA_ARENA - ZONA_DE_COMBATE_Y) / 2.0f;

const float LIMITE_IZQ_ARENA = ARENA_MARGEN_X;
const float LIMITE_DCH_ARENA = ARENA_MARGEN_X + ZONA_DE_COMBATE_X;
const float LIMITE_ARRIBA_ARENA = ARENA_MARGEN_Y;
const float LIMITE_ABAJO_ARENA = ARENA_MARGEN_Y + ZONA_DE_COMBATE_Y;

const int NUM_DE_BARRERAS = 8;

const int ARRIBA = 0;
const int ABAJO = 1;
const int IZQUIERDA = 2;
const int DERECHA = 3;

class Arena
{
    Animal* combatientes_[2]{};
    float pos_x_[2] = {};
    float pos_y_[2] = {};
    float pos_antigua_x_[2] = {};
    float pos_antigua_y_[2] = {};
    float ultima_direccion_x_[2] = {};
    float ultima_direccion_y_[2] = {};
    bool  vivo_[2] = {};

    bool movimiento_arriba_[2] = {};
    bool movimiento_abajo_[2] = {};
    bool movimiento_izq_[2] = {};
    bool movimiento_dch_[2] = {};

    float recarga_de_ataque_[2] = {};

    float barrera_x_[NUM_DE_BARRERAS] = {};
    float barrera_y_[NUM_DE_BARRERAS] = {};
    bool  barrera_visible_[NUM_DE_BARRERAS] = {};
    float contador_ciclo_barrera_[NUM_DE_BARRERAS] = {};
    float ciclo_maximo_barrera_[NUM_DE_BARRERAS] = {};

    int  ganador_ = -1;
    bool combate_terminado_ = false;

    void actualizarMovimiento(float dt);
    void actualizarAtaques(float dt);
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
    void recibirMovimiento(int jugador, int movimiento, bool tecla_pulsada);
    bool recibirAtaque(int jugador);
    int  obtenerPerdedor() const;
    bool combateTerminado() const;
    int  ganadorCombate()   const;

    // Getters para Renderizador
    bool  isBarreraVisible(int i) const { return barrera_visible_[i]; }
    float getBarreraX(int i)      const { return barrera_x_[i]; }
    float getBarreraY(int i)      const { return barrera_y_[i]; }
    bool  isVivo(int i)                   const { return vivo_[i]; }
    const Animal* getCombatiente(int i)   const { return combatientes_[i]; }
    const Ataque* getAtaqueObjeto(int i)  const {
        return combatientes_[i] ? combatientes_[i]->getAtaque() : nullptr;
    }
};