ç#pragma once
#include "ataque.h"

class Disparo : public Ataque {

    float velocidad_ = 200.0f;  // píxeles por segundo

public:
    Disparo(int dano, float alcance, float recarga,const char* sprite, float tamanio, float duracion,float r, float g, float b)
        : Ataque(dano, alcance, recarga, sprite, tamanio, duracion, r, g, b) {}

    void mover(float dt) override {
        if (!activo_) return;

        // Viaja en la dirección indicada
        x_ += dirX_ * velocidad_ * (dt / 1000.0f);
        y_ += dirY_ * velocidad_ * (dt / 1000.0f);

        tiempoActivo_ += dt / 1000.0f;

        // Se desactiva si llega al alcance máximo
        if (tiempoActivo_ >= alcance_ / velocidad_)
            desactivar();
    }
};