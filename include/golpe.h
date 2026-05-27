#pragma once
#include "ataque.h"

class Golpe : public Ataque {

public:
    Golpe(int dano, float alcance, float recarga,
        const char* sprite, float tamanio, float duracion,
        float r, float g, float b)
        : Ataque(dano, alcance, recarga, sprite, tamanio, duracion, r, g, b) {}

    void mover(float dt) override {
        if (!activo_) return;

        tiempoActivo_ += dt / 1000.0f;

        // El golpe aparece y desaparece tras su duración
        if (tiempoActivo_ >= duracion_visual_)
            desactivar();
    }
};