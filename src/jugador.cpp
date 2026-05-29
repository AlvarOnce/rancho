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
        animales_.push_back(new Granjero({4,0}, 0));
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
         animales_.push_back(new Gallina({ i,7}, 1));
    }

   /* if (equipo_ == 1) 
    {
        for (int i = 0; i < 9; i++)
            animales_.push_back(new Cabra(480.0f + 44.0f + 15.0f * i - 11.0f, 36.0f + 176.0f - (22.0f * i) + 11.0f, -3.0f - 0.01f * i, 20.0f, 152.0f + 22.0f * 8.0f, 1));

         for (int i = 9; i < 18; i++)
            animales_.push_back(new Gallina(480.0f + 15.0f * (i - 9.0f) - 11.0f, 36.0f + 176.0f - (22.0f * (i - 9.0f)) + 11.0f, -3.0f - 0.01f * (i - 9.0f) - 0.08f, 20.0f, 152.0f + 22.0f * 7.0f, 1));
    }*/
    
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

    //template <typename T> void agregarAnimal(float& posX, float& posY, float& posZ, float dirX, float varExt2) {
    //    // 1. Crea el animal en la posición actual del cursor
    //    animales_.push_back(new T(posX, posY, posZ, 20.0f, varExt2, equipo_));

    //    // 2. Avanza el cursor automáticamente para el SIGUIENTE animal (+1 en X)
    //    posX += dirX * 1.0f;  // El paso de +1 que necesitas
    //    posY -= 22.0f;        // Mantiene el desplazamiento en Y
    //    posZ -= 0.01f;        // Mantiene el desplazamiento en Z
    //}
