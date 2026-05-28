#pragma once
#include "animal.h"
#include "ondaa.h"

class Cabra : public Animal {

public:
    Cabra(float posx, float posy, float capa, int vida, float xinicial, int equipo)
        : Animal(posx, posy, capa, vida, xinicial, equipo) {

        max_casillas_movidas_ = 4;
        nFrames = 5;

        ataque_ = new Onda(7, 100.0f, 0.6f, "../assets/Sprites/creditos/botella.png", 48.0f, 0.4f, 1.0f, 0.0f, 1.0f);
    }

    const char* getTipoAtaque() const override { return "Onda"; }

    void dibujar(Renderizador* motor) override;
};