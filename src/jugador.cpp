#include "jugador.h"
#include "oveja.h"
#include "gallina.h"

Jugador::Jugador(int equipo) : equipo_(equipo) {

    if (equipo_ == 0) {

        for (int i = 0; i < 9; i++)
            animales_[i] = new Oveja(-44 - 15 * i + 11,36 + 176 - (22 * i) + 11,-3 - 0.01 * i,20,152,0);

        for (int i = 9; i < 18; i++)
            animales_[i] = new Gallina(-15 * (i - 9) + 11,36 + 176 - (22 * (i - 9)) + 11,-3 - 0.01 * (i - 9) - 0.08,20,152 + 22,0);
    }

    if (equipo_ == 1) {

        for (int i = 0; i < 9; i++)
            animales_[i] = new Oveja(480 + 44 + 15 * i - 11,36 + 176 - (22 * i) + 11,-3 - 0.01 * i,20,152 + 22 * 8,1);

         for (int i = 9; i < 18; i++)
            animales_[i] = new Gallina(480 + 15 * (i - 9) - 11,36 + 176 - (22 * (i - 9)) + 11,-3 - 0.01 * (i - 9) - 0.08,20,152 + 22 * 7,1);
    }
}

Jugador::~Jugador() {
    for (int i = 0; i < numAnimales_; i++)
        delete animales_[i];
}