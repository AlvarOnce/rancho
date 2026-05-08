#include "creditos.h"

void GallinaCreditos::animar(float dt) {

    timer = timer + dt;
    if (timer > msStep)
    {
        if (frameActualX_ < nFrames - 1) frameActualX_++;
        else frameActualX_ = 0;
        timer = timer - msStep;
    }
}

void BotellaCreditos::animar(float dt) {

    timer = timer + dt;
    if (timer > msStep)
    {
        if (frameActualX_ < nFrames - 1) frameActualX_++;
        else if (loop) frameActualX_ = 0;

        timer = timer - msStep;
    }
}

void Creditos::reset() {

    posx_ = 726;
    posy_ = 135;

    botella.posx_ = 364;

    fin_ = false;
}

void Creditos::actualizar(float dt) {

    if (posx_ > posFinal_)
    {
        posx_ -= 0.8;
        botella.posx_ -= 0.8;
    }
    else
        fin_ = true;

    gallina.animar(dt);

    if(gallina.posx_ > botella.posx_ - 54)
    botella.animar(dt);

}


void Creditos::dibujar(Renderizador* motor) {

    motor->dibujarSprite("../assets/Sprites/creditos/creditos.png", 2048, 512, posx_, posy_, -1);
    motor->dibujarSprite("../assets/Sprites/creditos/botella.png", 3 * 256, 3 * 32, botella.posx_, botella.posy_, -1.9, 1, 8, botella.frameActualX_, botella.frameActualY_);
    motor->dibujarSprite("../assets/Sprites/creditos/gallinaCreditos.png", 3 * 256, 3 * 32, gallina.posx_, gallina.posy_, -2, 1, 8, gallina.frameActualX_, gallina.frameActualY_);
}


