#pragma once
#include "ataque.h"

class Golpe : public Ataque {

public:
    // Golpe genérico
    Golpe(int dano = 10) {
        dano_ = dano;
        alcance_ = 30.0f;
        tipo_ = "Golpe";
        sprite_ = "../assets/Sprites/ataques/punetazo.png";
        tamanio_ = 32.0f;
        duracion_ = 0.2f;
        r_ = 1.0f;  g_ = 0.5f;  b_ = 0.0f;  // naranja
    }

    // Golpe personalizado (para pasar parámetros específicos)
    Golpe(int dano, float alcance, const char* sprite, float tamanio, float duracion, float r, float g, float b) {
        dano_ = dano;
        alcance_ = alcance;
        tipo_ = "Golpe";
        sprite_ = sprite;
        tamanio_ = tamanio;
        duracion_ = duracion;
        r_ = r;  g_ = g;  b_ = b;
    }
};