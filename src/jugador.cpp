#include "jugador.h"
#include "granjero.h"
#include "cabra.h"
#include "oveja.h"
#include "cerdo.h"
#include "llama.h"
#include "gallina.h"
#include "estructuras.h"

Jugador::Jugador(int equipo) : equipo_(equipo) // esto de construir los animales con tantos números mágicos es muy feo, mejor usar filas y columnas
{
    //float posXinicial = -44.0f - 15.0f * 0 + 11.0f;
    //float posYinicial = 36.0f + 176.0f - (22.0f * 0) + 11.0f;
    //float capaInicial = -3.0f - 0.01f * 0;

    if (equipo_ == 0)
    {
        animales_.push_back(new Cabra({ 0,0 }, 0));
        animales_.push_back(new Oveja({ 1,0 }, 0));
        animales_.push_back(new Cerdo({ 2,0 }, 0));
        animales_.push_back(new Llama({ 3,0 }, 0));
        animales_.push_back(new Granjero({ 4,0 }, 0));
        animales_.push_back(new Llama({ 5,0 }, 0));
        animales_.push_back(new Cerdo({ 6,0 }, 0));
        animales_.push_back(new Oveja({ 7,0 }, 0));
        animales_.push_back(new Cabra({ 8,0 }, 0));

        for (int i = 0; i < 9; i++)
            animales_.push_back(new Gallina({ i,1 }, 0));
    }

    if (equipo_ == 1)
    {
        animales_.push_back(new Cabra({ 0,8 }, 1));
        animales_.push_back(new Oveja({ 1,8 }, 1));
        animales_.push_back(new Cerdo({ 2,8 }, 1));
        animales_.push_back(new Llama({ 3,8 }, 1));
        animales_.push_back(new Granjero({ 4,8 }, 1));
        animales_.push_back(new Llama({ 5,8 }, 1));
        animales_.push_back(new Cerdo({ 6,8 }, 1));
        animales_.push_back(new Oveja({ 7,8 }, 1));
        animales_.push_back(new Cabra({ 8,8 }, 1));

        for (int i = 0; i < 9; i++)
            animales_.push_back(new Gallina({ i,7 }, 1));
    }

    animalEnCombate_ = animales_[9];
}

Jugador::~Jugador()
{
    // recorrer el vector y liberar memoria
    for (Animal* animal : animales_) {
        delete animal;
    }

    animales_.clear(); // para dejar el vector vacío
}

    //template <typename T> void agregarAnimal(float& posX, float& posY, float& posZ, float dirX, float varExt2) {
    //    // 1. Crea el animal en la posición actual del cursor
    //    animales_.push_back(new T(posX, posY, posZ, 20.0f, varExt2, equipo_));

    //    // 2. Avanza el cursor automáticamente para el SIGUIENTE animal (+1 en X)
    //    posX += dirX * 1.0f;  // El paso de +1 que necesitas
    //    posY -= 22.0f;        // Mantiene el desplazamiento en Y
    //    posZ -= 0.01f;        // Mantiene el desplazamiento en Z
    //}
