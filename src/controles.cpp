#include "controles.h"

void PalomaCreditos::animar(float dt) {

    timer = timer + dt;
    if (timer > msStep)
    {
        if (frameActualX_ < nFrames - 1) frameActualX_++;
        else frameActualX_ = 0;
        timer = timer - msStep;
    }
}

void PalomaCreditos::setState(int frameX, int frameY) {

    frameActualX_ = frameX;
    frameActualY_ = frameY;

}


void Controles::reset() {

    posx_ = 755;
    posy_ = 135;

    fin_ = false;
}

void Controles::actualizar(float dt) {

    if (posx_ > posFinal_)
    {
       // posx_ -= 0.8;
 
    }
    else
        fin_ = true;

    if (paloma.posy_ > 53) {

        paloma.posy_ -= 2;
        paloma.animar(dt);

    }
    else
        paloma.setState(0,0);

}

void Controles::dibujar(Renderizador* motor) {

    motor->dibujarSprite("../assets/Sprites/controles/fondo.png", 3*512, 3*128, posx_, posy_, -1);
    motor->dibujarSprite("../assets/Sprites/controles/palomaControles.png", 3*256, 3*128, paloma.posx_, paloma.posy_, -2, 4,8,paloma.frameActualX_,paloma.frameActualY_);

}
