#pragma once
#include "animal.h"
#include "estructuras.h"
#include "golpe.h"

class Gallina : public Animal {

public:
    Gallina(float posx, float posy, float capa, int vida, float xinicial, int equipo)
        : Animal(posx, posy, capa, vida, xinicial, equipo) {

        max_casillas_movidas_ = 2;
        nFrames = 8;

        ataque_ = new Golpe(15, 30.0f, 0.5f,
            "../assets/Sprites/ataques/golpe.png",
            32.0f, 0.2f,
            1.0f, 0.5f, 0.0f);
    }

    const char* getTipoAtaque() const override { return "Golpe"; }

    void dibujar(Renderizador* motor) override;
};