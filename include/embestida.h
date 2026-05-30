#pragma once
#include "ataque.h"

// este ataque no es un proyectil,es mas bien un estado del animal
// Arena la lee con isEmbestida() para saber que debe mover al animal rapidamente.
// El daño se aplica por contacto en confirmarImpacto cuando el animal está embistiendo.

class Embestida : public Ataque {

    float velocidad_embestida_ = 300.0f; // px/seg durante la embestida
    float duracion_embestida_;           // segundos que dura el impulso

public:
    Embestida(int dano, float duracion, float recarga, const char* sprite, float tamanio)
        : Ataque(dano, 0.0f, recarga, sprite, tamanio, duracion),
        duracion_embestida_(duracion) {}

    // Devuelve la velocidad del impulso para que arena la aplique al animal
    float getVelocidadEmbestida() const { return velocidad_embestida_; }
    float getDirX() const { return dirX_; }
    float getDirY() const { return dirY_; }

    // La embestida es un tipo especial — arena lo detecta con esto
    bool isEmbestida() const { return true; }

    void mover(float dt) override {
        if (!activo_) return;
        tiempoActivo_ += dt / 1000.0f;
        if (tiempoActivo_ >= duracion_embestida_)
            desactivar();
    }
};
