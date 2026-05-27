#pragma once
#include <cmath>
#include "ataque.h"
#include "animal.h"

class Interaccion {

public:

    // Detecta si el área del ataque toca al rival
    static bool hayColision(Ataque* ataque, Animal* rival) {
        if (!ataque || !ataque->isActivo()) return false;
        if (!rival) return false;

        float dx = ataque->getX() - rival->getPosX();
        float dy = ataque->getY() - rival->getPosY();
        float distancia = sqrt(dx * dx + dy * dy);

        return distancia < ataque->getTamanio() / 2;
    }
};