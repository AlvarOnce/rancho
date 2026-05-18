#include "jugador.h"
#include "renderizador.h"

Jugador::Jugador() : numAnimales(0) {
    for (int i = 0; i < 9; i++) {
        animales[i] = nullptr;
    }
}

Jugador::~Jugador() {
    limpiar();
}

void Jugador::inicializar(int numAnimales) {
    this->numAnimales = numAnimales;
}

void Jugador::agregarAnimal(int indice, Animal* animal) {
    if (indice >= 0 && indice < 9) {
        animales[indice] = animal;
    }
}

void Jugador::limpiar() {
    for (int i = 0; i < 9; i++) {
        if (animales[i] != nullptr) {
            delete animales[i];
            animales[i] = nullptr;
        }
    }
}

void Jugador::actualizar(float dt) {
    for (int i = 0; i < 9; i++) {
        if (animales[i] != nullptr) {
            animales[i]->actualizar(dt);
        }
    }
}

void Jugador::dibujar(Renderizador* motor) {
    for (int i = 0; i < 9; i++) {
        if (animales[i] != nullptr) {
            animales[i]->dibujar(motor);
        }
    }
}