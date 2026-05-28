#pragma once
#include <cmath>
#include "renderizador.h"

class Ataque {

protected:
    // Caracteristicas
    int dano_;
    float alcance_;
    float recarga_;

    // Visual
    const char* sprite_;
    float tamanio_;
    float duracion_visual_;
    float r_, g_, b_;

    // Posición y movimiento
    float x_ = 0;
    float y_ = 0;
    float dirX_ = 0;
    float dirY_ = 0;
    bool activo_ = false;
    float tiempoActivo_ = 0;

public:
    Ataque(int dano, float alcance, float recarga,
        const char* sprite, float tamanio, float duracion_visual,
        float r, float g, float b)
        : dano_(dano), alcance_(alcance), recarga_(recarga),
        sprite_(sprite), tamanio_(tamanio), duracion_visual_(duracion_visual),
        r_(r), g_(g), b_(b) {}

    virtual ~Ataque() {}

    // Getters estadísticas
    int getDano() const { return dano_; }
    float getAlcance() const { return alcance_; }
    float getRecarga() const { return recarga_; }

    // Getters visual
    const char* getSprite() const { return sprite_; }
    float getTamanio() const { return tamanio_; }
    float getDuracionVisual() const { return duracion_visual_; }
    void getColor(float& r, float& g, float& b) const {
        r = r_; g = g_; b = b_;
    }

    // Getters posición
    float getX() const { return x_; }
    float getY() const { return y_; }
    bool isActivo() const { return activo_; }

    // Dibuja el sprite del ataque en su posición actual
    virtual void dibujar(Renderizador* renderizador) const {
        if (!activo_) return;
        renderizador->dibujarSprite(sprite_, tamanio_, tamanio_, x_, y_, -2.0f, 1, 1, 0, 0, true);
    }

    // Activar el ataque en una posición con una dirección
    virtual void activar(float x, float y, float dirX, float dirY) {
        x_ = x;
        y_ = y;
        dirX_ = dirX;
        dirY_ = dirY;
        activo_ = true;
        tiempoActivo_ = 0;
    }

    void desactivar() {
        activo_ = false;
        tiempoActivo_ = 0;
    }

    // Cada subclase define cómo se mueve y cuándo se desactiva
    virtual void mover(float dt) = 0;
};