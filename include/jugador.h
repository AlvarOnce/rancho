#pragma once
#include "animal.h"

class Jugador {

    Animal* animales_[18];
    Animal* animalEnCombate_ = nullptr;
    int equipo_;           // 0 luz, 1 oscuridad
    int numAnimales_ = 18;

public:

    Jugador(int equipo);
    ~Jugador();

    Animal** getAnimales() { return animales_; }
    Animal* getAnimalEnCombate() { return animalEnCombate_; }
    void setAnimalEnCombate(Animal* animal) { animalEnCombate_ = animal; }
    int getEquipo() const { return equipo_; }
};