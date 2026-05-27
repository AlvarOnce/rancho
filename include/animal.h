#pragma once
#include <iostream>
#include <vector>
#include "ETSIDI.h"
#include "renderizador.h"
#include "estructuras.h"
#include "ataque.h"

enum modoJuego { TABLERO, BATALLA, CANCELAR };
enum animacionTipo { QUIETO, CAMINAR, ATACAR };

class Animal {

public:
    Animal(float posx, float posy, float capa, int vida, float xinicial, int equipo)
        : posx_(posx), posy_(posy), capaz_(capa), vida_(vida), xinicial_(xinicial), equipo_(equipo) {}

    virtual ~Animal() {
        if (ataque_) delete ataque_;
    }

    // ===== ATAQUE =====
    Ataque* ataque_ = nullptr;

    Ataque* getAtaque() const { return ataque_; }

    int getDanoAtaque() const {
        return ataque_ ? ataque_->getDano() : 0;
    }

    float getAlcanceAtaque() const {
        return ataque_ ? ataque_->getAlcance() : 0;
    }

    float getRecargaAtaque() const {
        return ataque_ ? ataque_->getRecarga() : 0;
    }

    virtual const char* getTipoAtaque() const = 0;

    void recibirDano(int dano) {
        vida_ -= dano;
    }

    void atacar(Animal* rival) {
        if (ataque_ && rival) {
            rival->recibirDano(getDanoAtaque());
        }
    }

    // ===== LÓGICA =====
    float posx_{ 100 };
    float posy_{ 100 };
    float capaz_;
    float velx_{ 0 };
    float vely_{ 0 };
    int equipo_;
    int vida_;
    float avanzando_casilla_ = 0;
    bool en_movimiento_ = false;
    int casillas_movidas_x_ = 0;
    int casillas_movidas_y_ = 0;
    int casillas_movidas_ = 0;
    int max_casillas_movidas_ = 100;
    bool intro_tablero_ = true;
    float xinicial_ = 152;

    int casillaInicial_[2] = { 0, 0 };

    bool mover(modoJuego modo, int dx, int dy);

    float getPosX() const { return posx_; }
    float getPosY() const { return posy_; }
    float getVelX() const { return velx_; }
    float getVelY() const { return vely_; }
    int   getEquipo() const { return equipo_; }
    bool  getEnMovimiento() const { return en_movimiento_; }
    int   getMaxCasillasMovidas() const { return max_casillas_movidas_; }
    bool  getIntroTablero() const { return intro_tablero_; }

    void setPosX(float posx) { posx_ = posx; }
    void setPosy(float posy) { posy_ = posy; }
    void setVelX(float velx) { velx_ = velx; }
    void setVelY(float vely) { vely_ = vely; }
    void setEnMovimiento(bool en_movimiento) { en_movimiento_ = en_movimiento; }

    // ===== DIBUJO Y ANIMACIÓN =====
    int frameActualX_ = 0;
    int frameActualY_ = 1;
    int nFrames = 1;
    float timer = 0;
    float msStep = 100;
    bool pausa = true;
    int ancho = 0;
    int alto = 0;

    void setState(int frameX, int frameY);
    void animar(float dt);

    virtual void actualizar(float dt);
    virtual void dibujar(Renderizador* motor);
    virtual std::vector<Movimiento> movimientosPosibles() const;
};