#pragma once
#include <iostream>
#include <vector>
#include "ETSIDI.h"
#include "estructuras.h"
#include "ataque.h"

// renderizador.h NO se incluye aquí para evitar include circular
// renderizador.h incluye animal.h, no al revés

enum modoJuego { TABLERO, BATALLA, CANCELAR };

enum especieAnimal { CABRA, CERDO, GALLINA, OVEJA, GRANJERO };

class Animal
{
public:
    Animal(float posx, float posy, float capa, int vida, float xinicial, int equipo)
        : posicion_(posx, posy), capaz_(capa), vida_(vida), xinicial_(xinicial), equipo_(equipo)
    {
        if (equipo_ == 0) setState(0, 0);
        else              setState(0, 1);
    }

    virtual ~Animal() {
        if (ataque_) delete ataque_;
    }

    // ===== ATAQUE =====
    Ataque* ataque_ = nullptr;
    Ataque* getAtaque() const { return ataque_; }
    int   getDanoAtaque()    const { return ataque_ ? ataque_->getDano() : 0; }
    float getAlcanceAtaque() const { return ataque_ ? ataque_->getAlcance() : 0.f; }
    float getRecargaAtaque() const { return ataque_ ? ataque_->getRecarga() : 0.f; }
    virtual const char* getTipoAtaque() const = 0;
    void recibirDano(int dano) { vida_ -= dano; }

    // ===== LÓGICA =====
    Vector2D posicion_;
    Vector2D velocidad_;
    float capaz_;
    int   equipo_;
    int   vida_;
    float avanzando_casilla_ = 0;
    bool  en_movimiento_ = false;
    int   casillas_movidas_x_ = 0;
    int   casillas_movidas_y_ = 0;
    int   casillas_movidas_ = 0;
    int   max_casillas_movidas_ = 100;
    bool  intro_tablero_ = true;
    float xinicial_ = 152;
    int   casillaInicial_[2] = { 0, 0 };
    especieAnimal especie_ = {};

    bool mover(modoJuego modo, int dx, int dy);

    float getPosX()  const { return posicion_.x; }
    float getPosY()  const { return posicion_.y; }
    float getVelX()  const { return velocidad_.x; }
    float getVelY()  const { return velocidad_.y; }
    float getCapaz() const { return capaz_; }
    int   getEquipo()           const { return equipo_; }
    bool  getEnMovimiento()     const { return en_movimiento_; }
    int   getMaxCasillasMovidas() const { return max_casillas_movidas_; }
    bool  getIntroTablero()     const { return intro_tablero_; }
    int   getFrameActualX()     const { return frameActualX_; }
    int   getFrameActualY()     const { return frameActualY_; }
    especieAnimal getEspecie()  const { return especie_; }
    bool  getVivo()             const { return vida_ > 0; }

    void setPosicion(const Vector2D& pos) { posicion_ = pos; }
    void setPosX(float x) { posicion_.x = x; }
    void setPosy(float y) { posicion_.y = y; }
    void setVelocidad(const Vector2D& vel) { velocidad_ = vel; }
    void setVelX(float x) { velocidad_.x = x; }
    void setVelY(float y) { velocidad_.y = y; }
    void setEnMovimiento(bool v) { en_movimiento_ = v; }

    // ===== ANIMACIÓN =====
    int   frameActualX_ = 0;
    int   frameActualY_ = 1;
    int   nFrames = 1;
    float timer = 0;
    float msStep = 100;
    bool  pausa = true;

    void setState(int frameX, int frameY);
    void animar(float dt);

    virtual void actualizar(float dt);
    virtual std::vector<Movimiento> movimientosPosibles() const;
};