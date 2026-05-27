#pragma once
#include "estructuras.h"

class PalomaControles
{
private:

    float timer_ = 0;
    float msStep_ = 100;
    int nFrames_ = 5;

    Vector2D posicion_ = { 55, 315 };
    int frameActualX_ = 0;
    int frameActualY_ = 2;

public:

    void animar(float dt);
    void setState(int frameX, int frameY);

    float getPosX() const { return posicion_.x; }
    float getPosY() const { return posicion_.y; }
    void setPosX(float posx) { posicion_.x = posx; }
    void setPosY(float posy) { posicion_.y = posy; }
    int getFrameActualX() const { return frameActualX_; }
    int getFrameActualY() const { return frameActualY_; }
};

class Controles
{
private:

    PalomaControles paloma_;
    Vector2D posicion_;
    const float posFinal_ = -246;
    float capaz_;
    bool fin_;

public:

    void reset();
    Controles() { reset(); }
    bool getFinalizado() const { return fin_; }

    void actualizar(float dt);

    float getPosX() const { return posicion_.x; }
    float getPosY() const { return posicion_.y; }
	void setPosX(float posx) { posicion_.x = posx; }
	void setPosY(float posy) { posicion_.y = posy; }

    const PalomaControles& getPaloma() const { return paloma_; }
};