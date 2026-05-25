#pragma once
#include "animal.h"
#include "estructuras.h"

class Jugador 
{
	// esto nos lo dijo psanse en la tutoria:
	//std::vector<Animal*> animales_ // para poder usar push_back y no tener que escribir 18 veces new Oveja/Gallina/Cabra/Cerdo/Granjero/Llama
	Animal* animales_[Constantes::NUMERO_ANIMALES] = { nullptr };
    Animal* piezaSeleccionada_ = nullptr;
    Animal* animalEnCombate_ = nullptr;
    int equipo_;

public:

    Jugador(int equipo);
    ~Jugador();

    // Animales
    Animal* const* getAnimales() const { return animales_; }
    Animal* getAnimalEnCombate() { return animalEnCombate_; }
    void setAnimalEnCombate(Animal* animal) { animalEnCombate_ = animal; }

    // Pieza seleccionada
    void agarrarPieza(Animal* p) { piezaSeleccionada_ = p; }
    void soltarPieza() { piezaSeleccionada_ = nullptr; }
    bool tienePiezaAgarrada() const { return piezaSeleccionada_ != nullptr; }
    Animal* getPiezaSeleccionada() const { return piezaSeleccionada_; }

    // Identidad
    int getEquipo() const { return equipo_; }
};