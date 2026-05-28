#pragma once
#include "animal.h"
#include "estructuras.h"
#include "disparoo.h"

class Gallina : public Animal {

public:
    Gallina(float posx, float posy, float capa, int vida, float xinicial, int equipo)
        : Animal(posx, posy, capa, vida, xinicial, equipo) {

        max_casillas_movidas_ = 2;
        nFrames = 8;

        ataque_ = new Disparo(12, 140.0f, 0.8f, "../assets/Sprites/creditos/botella.png", 20.0f, 0.35f, 1.0f, 1.0f, 0.0f);
    }

    const char* getTipoAtaque() const override { return "Golpe"; }

    void dibujar(Renderizador* motor) override;
};
