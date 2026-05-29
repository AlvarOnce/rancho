#include "interaccion.h"

bool Interaccion::hayColision(Ataque* ataque, Animal* rival) {
    if (!ataque || !ataque->isActivo() || !rival) return false;
    float dx = ataque->getX() - rival->getPosX();
    float dy = ataque->getY() - rival->getPosY();
    return sqrt(dx * dx + dy * dy) < ataque->getTamanio() / 2.0f + 11.0f;
}
bool Interaccion::procesarImpacto(Ataque* ataque, Animal* rival) {
    if (!hayColision(ataque, rival)) return false;
    rival->recibirDano(ataque->getDano());
    ataque->desactivar();
    return true;
}