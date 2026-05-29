#pragma once
#include "ataque.h"

class Onda : public Ataque {

    float expansion_ = 50.0f;  // píxeles por segundo que crece
    float tamanioInicial_;

public:
    Onda(int dano, float alcance, float recarga, const char* sprite, float tamanio, float duracion, float r, float g, float b)
        : Ataque(dano, alcance, recarga, sprite, tamanio, duracion, r, g, b),
        tamanioInicial_(tamanio) {}

    void activar(float x, float y, float dirX, float dirY) override {
        Ataque::activar(x, y, dirX, dirY);
        tamanio_ = tamanioInicial_;  // resetear tamaño al activar
    }

    void mover(float dt) override {
        if (!activo_) return;

        // La onda crece con el tiempo
        tamanio_ += expansion_ * (dt / 1000.0f);
        tiempoActivo_ += dt / 1000.0f;

        if (tiempoActivo_ >= duracion_visual_)
            desactivar();
    }
};