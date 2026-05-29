#pragma once
#include "ataque.h"

class Disparo : public Ataque {

    float velocidad_ = 200.0f;  // píxeles por segundo

public:
    Disparo(int dano, float alcance, float recarga, const char* sprite, float tamanio, float duracion, float r, float g, float b)
        : Ataque(dano, alcance, recarga, sprite, tamanio, duracion, r, g, b) {}

    void mover(float dt) override {
        if (!activo_) return;

        // Se desplaza en la dirección activada
        x_ += dirX_ * velocidad_ * (dt / 1000.0f);
        y_ += dirY_ * velocidad_ * (dt / 1000.0f);

        tiempoActivo_ += dt / 1000.0f;

        // Se desactiva al llegar al alcance máximo
        if (tiempoActivo_ >= alcance_ / velocidad_)
            desactivar();
    }
};