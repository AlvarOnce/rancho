#pragma once
#include "estructuras.h"

class BotellaCreditos
{
private:
    float timer_ = 0;
    float msStep_ = 100;
    int nFrames_ = 8;
    bool loop_ = false;

    Vector2D posicion_ = { 364, 25 };
    int frameActualX_ = 0;
    int frameActualY_ = 1;

public:

    void animar(float dt);

    float getPosX() const { return posicion_.x; }
    float getPosY() const { return posicion_.y; }
    void setPosX(float posx) { posicion_.x = posx; }
    void setPosY(float posy) { posicion_.y = posy; }
    int getFrameActualX() const { return frameActualX_; }
    int getFrameActualY() const { return frameActualY_; }
};


class GallinaCreditos
{
private:

    float timer_ = 0;
    float msStep_ = 100;
    int nFrames_ = 8;

    const float posx_ = 40;
    const float posy_ = 37;
    int frameActualX_ = 0;
    int frameActualY_ = 1;

public:

    void animar(float dt);

    float getPosX() const { return posx_; }
    float getPosY() const { return posy_; }
    int getFrameActualX() const { return frameActualX_; }
    int getFrameActualY() const { return frameActualY_; }
};


class Creditos
{
private:

    BotellaCreditos botella_;
    GallinaCreditos gallina_;
    Vector2D posicion_;
    const float posFinal_ = -246;
    float capaz_;
    bool fin_;

public:

    void reset();
    Creditos() { reset(); }
    bool getFinalizado() const { return fin_; }
    void actualizar(float dt);

    float getPosX() const { return posicion_.x; }
    float getPosY() const { return posicion_.y; }
    void setPosX(float posx) { posicion_.x = posx; }
    void setPosY(float posy) { posicion_.y = posy; }

    const BotellaCreditos& getBotella() const { return botella_; }
    const GallinaCreditos& getGallina() const { return gallina_; }
};