#include "controles.h"

void PalomaControles::animar(float dt) 
{
    timer_ = timer_ + dt;
    if (timer_ > msStep_)
    {
        if (frameActualX_ < nFrames_ - 1) frameActualX_++;
        else frameActualX_ = 0;
        timer_ = timer_ - msStep_;
    }
}

void PalomaControles::setState(int frameX, int frameY) 
{
    frameActualX_ = frameX;
    frameActualY_ = frameY;
}


void Controles::reset() {

    posicion_.x = 755;
    posicion_.y = 135;

    fin_ = false;
}

void Controles::actualizar(float dt) {

    if (posicion_.x > posFinal_)
    {
       // posx_ -= 0.8;
 
    }
    else
        fin_ = true;

    if (paloma_.getPosY() > 53) {

        paloma_.setPosY(paloma_.getPosY() - 2);
        paloma_.animar(dt);

    }
    else
        paloma_.setState(0,0);
}