#pragma once
#include <cmath>

// El dibujo del ataque lo hace renderizador.cpp usando getSprite(), getX(), getY(), etc.

class Ataque {
protected:
    int         dano_;
    float       alcance_;
    float       recarga_;
    const char* sprite_;
    float       tamanio_;
    float       duracion_visual_;
    float       r_, g_, b_;

    float x_ = 0, y_ = 0;
    float dirX_ = 0, dirY_ = 0;
    bool  activo_ = false;
    float tiempoActivo_ = 0;

public:
    Ataque(int dano, float alcance, float recarga, const char* sprite, float tamanio, float duracion_visual, float r, float g, float b)
        : dano_(dano), alcance_(alcance), recarga_(recarga),
        sprite_(sprite), tamanio_(tamanio), duracion_visual_(duracion_visual),
        r_(r), g_(g), b_(b) {}

    virtual ~Ataque() {}

    int         getDano()    const { return dano_; }
    float       getAlcance() const { return alcance_; }
    float       getRecarga() const { return recarga_; }
    const char* getSprite()  const { return sprite_; }
    float       getTamanio() const { return tamanio_; }
    float       getX()       const { return x_; }
    float       getY()       const { return y_; }
    bool        isActivo()   const { return activo_; }
    void getColor(float& r, float& g, float& b) const { r = r_; g = g_; b = b_; }

    virtual void activar(float x, float y, float dirX, float dirY) {
        x_ = x; y_ = y; dirX_ = dirX; dirY_ = dirY;
        activo_ = true; tiempoActivo_ = 0;
    }

    void desactivar() { activo_ = false; tiempoActivo_ = 0; }

    virtual void mover(float dt) = 0;
};