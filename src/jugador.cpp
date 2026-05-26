#include "jugador.h"
#include "oveja.h"
#include "gallina.h"
#include "cabra.h"

Jugador::Jugador(int equipo) : equipo_(equipo) 
{
    if (equipo_ == 0) 
    {
        for (int i = 0; i < 9; i++)
            animales_[i] = new Cabra(-44.0f - 15.0f * i + 11.0f,36.0f + 176.0f - (22.0f * i) + 11.0f,-3.0f - 0.01f * i,20.0f,152.0f,0);

        for (int i = 9; i < 18; i++)
            animales_[i] = new Gallina(-15.0f * (i - 9.0f) + 11.0f,36.0f + 176.0f - (22.0f * (i - 9.0f)) + 11.0f,-3.0f - 0.01f * (i - 9.0f) - 0.08f,20.0f,152.0f + 22,0);
    }

    if (equipo_ == 1) 
    {
        for (int i = 0; i < 9; i++)
            animales_[i] = new Cabra(480.0f + 44.0f + 15.0f * i - 11.0f,36.0f + 176.0f - (22.0f * i) + 11.0f,-3.0f - 0.01f * i,20.0f,152.0f + 22.0f * 8.0f,1);

         for (int i = 9; i < 18; i++)
            animales_[i] = new Gallina(480.0f + 15.0f * (i - 9.0f) - 11.0f,36.0f + 176.0f - (22.0f * (i - 9.0f)) + 11.0f,-3.0f - 0.01f * (i - 9.0f) - 0.08f,20.0f,152.0f + 22.0f * 7.0f,1);
    }
    animalEnCombate_ = animales_[9];
}

Jugador::~Jugador() 
{
    for (int i = 0; i < Constantes::NUMERO_ANIMALES; i++)
        delete animales_[i];

	// cuando usemos el vector de psanse, esto se haría automáticamente al salir del ámbito, 
    // pero como usamos un array de punteros, debemos eliminar cada animal manualmente para evitar fugas de memoria
    //animales_.clear();    
}