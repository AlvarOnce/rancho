#include "jugador.h"
#include "gallina.h"
#include "cabra.h"

Jugador::Jugador(int equipo) : equipo_(equipo) // esto de construir los animales con tantos números mágicos es muy feo, mejor usar filas y columnas
{
    if (equipo_ == 0) 
    {
        for (int i = 0; i < 9; i++)
            animales_.push_back(new Cabra(-44.0f - 15.0f * i + 11.0f, 36.0f + 176.0f - (22.0f * i) + 11.0f, -3.0f - 0.01f * i, 20.0f, 152.0f, 0));

        for (int i = 9; i < 18; i++)
            animales_.push_back(new Gallina(-15.0f * (i - 9.0f) + 11.0f, 36.0f + 176.0f - (22.0f * (i - 9.0f)) + 11.0f, -3.0f - 0.01f * (i - 9.0f) - 0.08f, 20.0f, 152.0f + 22, 0));
    }

    if (equipo_ == 1) 
    {
        for (int i = 0; i < 9; i++)
            animales_.push_back(new Cabra(480.0f + 44.0f + 15.0f * i - 11.0f, 36.0f + 176.0f - (22.0f * i) + 11.0f, -3.0f - 0.01f * i, 20.0f, 152.0f + 22.0f * 8.0f, 1));

         for (int i = 9; i < 18; i++)
            animales_.push_back(new Gallina(480.0f + 15.0f * (i - 9.0f) - 11.0f, 36.0f + 176.0f - (22.0f * (i - 9.0f)) + 11.0f, -3.0f - 0.01f * (i - 9.0f) - 0.08f, 20.0f, 152.0f + 22.0f * 7.0f, 1));
    }
    
    // al haber hecho push_back en orden (primero 9 cabras y luego 9 gallinas), 
    // la primera gallina sigue estando en el índice 9, así que esta línea funciona igual
    animalEnCombate_ = animales_[9]; // de todas formas, esta variable se actualiza en el tablero cada vez que hay un combate, así que no es importante
}

Jugador::~Jugador()
{
    // recorrer el vector y liberar memoria
    for (Animal* animal : animales_) {
        delete animal;
    }

    animales_.clear(); // para dejar el vector vacío
}