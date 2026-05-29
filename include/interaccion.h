#pragma once
#include <cmath>
#include "ataque.h"
#include "animal.h"

class Interaccion {
public:
    static bool hayColision(Ataque* ataque, Animal* rival);
    static bool procesarImpacto(Ataque* ataque, Animal* rival);
};