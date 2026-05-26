#pragma once
#include "ataque.h"

class Disparo : public Ataque {

public:
    Disparo(int dano = 8) {
        dano_ = dano;
        alcance_ = 150.0f;
        tipo_ = "Disparo";
        sprite_ = "../assets/Sprites/ataques/proyectil.png";
        tamanio_ = 16.0f;
        duracion_ = 0.3f;
        r_ = 0.0f;  g_ = 1.0f;  b_ = 0.5f;  // cian
    }

    Disparo(int dano, float alcance, const char* sprite, float tamanio, float duracion, float r, float g, float b) {
        dano_ = dano;
        alcance_ = alcance;
        tipo_ = "Disparo";
        sprite_ = sprite;
        tamanio_ = tamanio;
        duracion_ = duracion;
        r_ = r;  g_ = g;  b_ = b;
    }
};