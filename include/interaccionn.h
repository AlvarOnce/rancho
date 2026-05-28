#pragma once
#include <cmath>
#include "ataquee.h"
#include "animal.h"

class Interaccion {

public:

    // Devuelve true si el ataque toca al rival (colisión circular)
    static bool hayColision(Ataque* ataque, Animal* rival) {
        if (!ataque || !ataque->isActivo()) return false;
        if (!rival) return false;

        float dx = ataque->getX() - rival->getPosX();
        float dy = ataque->getY() - rival->getPosY();
        float distancia = sqrt(dx * dx + dy * dy);

        // radio de colisión = mitad del tamaño del ataque + radio del animal (11px)
        float radioColision = ataque->getTamanio() / 2.0f + 11.0f;

        return distancia < radioColision;
    }

    // Procesa el impacto: aplica daño al rival y desactiva el ataque
    // Devuelve true si hubo impacto
    static bool procesarImpacto(Ataque* ataque, Animal* rival) {
        if (!hayColision(ataque, rival)) return false;
        rival->recibirDano(ataque->getDano());
        ataque->desactivar();
        return true;
    }
};
