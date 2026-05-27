#include "creditos.h"

void GallinaCreditos::animar(float dt) 
{
    timer_ = timer_ + dt;
    if (timer_ > msStep_)
    {
        if (frameActualX_ < nFrames_ - 1) frameActualX_++;
        else frameActualX_ = 0;
        timer_ = timer_ - msStep_;
    }
}

void BotellaCreditos::animar(float dt) 
{
    timer_ = timer_ + dt;
    if (timer_ > msStep_)
    {
        if (frameActualX_ < nFrames_ - 1) frameActualX_++;
        else if (loop_) frameActualX_ = 0;

        timer_ = timer_ - msStep_;
    }
}

void Creditos::reset() 
{
    posicion_.x = 726;
    posicion_.y = 135;

    botella_.setPosX(364);

    fin_ = false;
}

void Creditos::actualizar(float dt) 
{
    if (posicion_.x > posFinal_)
    {
        posicion_.x -= 0.8;
        botella_.setPosX(botella_.getPosX() - 0.8f);

    }
    else
        fin_ = true;

    gallina_.animar(dt);

    if (gallina_.getPosX() > botella_.getPosX() - 54)
    botella_.animar(dt);
}