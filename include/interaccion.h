#pragma once
#include "ataque.h"
#include "animal.h"
#include "embestida.h"

class Interaccion {
public:
    static bool hayColision(Ataque* ataque, const Animal* rival);
    static bool procesarImpacto(Ataque* ataque, Animal* rival);
    static bool procesarEmbestida(Embestida* emb, Animal* atacante, Animal* rival,float px_atac, float py_atac,float px_rival, float py_rival);
    static bool ataqueChocaBarrera(const Ataque* ataque, float bx, float by,float semi_ancho = 10.0f, float semi_alto = 12.0f);
    static bool animalChocaBarrera(float x, float y, float bx, float by,float semi_ancho = 10.0f, float semi_alto = 12.0f);
    static void mantenerDentroArena(float& x, float& y,float margen_x, float margen_y,float zona_x, float zona_y,float radio = 11.0f);
    static bool ataqueEstaFuera(const Ataque* ataque,float lim_izq, float lim_dch,float lim_arr, float lim_abj);
};
