#include "interaccion.h"
#include <cmath>

bool Interaccion::hayColision(Ataque* ataque, const Animal* rival) {
    if (!ataque || !ataque->isActivo() || !rival) return false;
    float dx = ataque->getX() - rival->getPosX();
    float dy = ataque->getY() - rival->getPosY();
    return sqrt(dx * dx + dy * dy) < ataque->getTamanio() / 2.0f + 11.0f;
}

bool Interaccion::procesarImpacto(Ataque* ataque, Animal* rival) {
    if (!hayColision(ataque, rival)) return false;
    rival->recibirDano(ataque->getDano());
    ataque->desactivar();
    return true;
}

bool Interaccion::procesarEmbestida(Embestida* emb, Animal* atacante, Animal* rival,
    float px_atac, float py_atac,
    float px_rival, float py_rival)
{
    if (!emb || !emb->isActivo() || !rival) return false;
    float dx = px_atac - px_rival;
    float dy = py_atac - py_rival;
    if (sqrt(dx * dx + dy * dy) < 22.0f) {
        rival->recibirDano(emb->getDano());
        emb->desactivar();
        return true;
    }
    return false;
}

bool Interaccion::ataqueChocaBarrera(const Ataque* ataque, float bx, float by,
    float semi_ancho, float semi_alto)
{
    if (!ataque || !ataque->isActivo()) return false;
    float dx = ataque->getX() - bx;
    float dy = ataque->getY() - by;
    return (dx > -semi_ancho && dx < semi_ancho &&
        dy > -semi_alto && dy < semi_alto);
}

bool Interaccion::animalChocaBarrera(float x, float y, float bx, float by,
    float semi_ancho, float semi_alto)
{
    float dx = x - bx;
    float dy = y - by;
    return (dx > -semi_ancho && dx < semi_ancho &&
        dy > -semi_alto && dy < semi_alto);
}

void Interaccion::mantenerDentroArena(float& x, float& y,
    float margen_x, float margen_y,
    float zona_x, float zona_y,
    float radio)
{
    if (x < margen_x + radio)          x = margen_x + radio;
    if (x > margen_x + zona_x - radio) x = margen_x + zona_x - radio;
    if (y < margen_y + radio)          y = margen_y + radio;
    if (y > margen_y + zona_y - radio) y = margen_y + zona_y - radio;
}

bool Interaccion::ataqueEstaFuera(const Ataque* ataque,
    float lim_izq, float lim_dch,
    float lim_arr, float lim_abj)
{
    if (!ataque || !ataque->isActivo()) return false;
    return (ataque->getX() < lim_izq || ataque->getX() > lim_dch ||
        ataque->getY() < lim_arr || ataque->getY() > lim_abj);
}