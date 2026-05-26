#pragma once
#include "ataque.h"

class Onda : public Ataque {

public:
    Onda(int dano = 6) {
        dano_ = dano;
        alcance_ = 100.0f;
        tipo_ = "Onda";
        sprite_ = "../assets/Sprites/ataques/onda.png";
        tamanio_ = 48.0f;
        duracion_ = 0.4f;
        r_ = 1.0f;  g_ = 0.0f;  b_ = 1.0f;  // magenta
    }

    Onda(int dano, float alcance, const char* sprite, float tamanio, float duracion, float r, float g, float b) {
        dano_ = dano;
        alcance_ = alcance;
        tipo_ = "Onda";
        sprite_ = sprite;
        tamanio_ = tamanio;
        duracion_ = duracion;
        r_ = r;  g_ = g;  b_ = b;
    }
};