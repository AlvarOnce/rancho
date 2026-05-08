#pragma once
#include "renderizador.h"

struct Letra {
    float horiz;
    float altura;
    float capa;
};

struct Tractor {
    float posx = 550;
    float posy = 65;
    int frameActualX_ = 0, frameActualY_ = 0;
    float timer{}, msStep = 300;
    int nFrames = 2;
    void animar(float dt);
};

struct Paloma {
    float posx = -10;
    float posy = 222;
    int frameActualX_ = 0, frameActualY_ = 1;
    float timer{}, msStep = 100;
    int nFrames = 4;
    void animar(float dt);
};

struct Nube {
    float posx;
    float posy;
    float capa;
};

struct Selector {
    float posx = 178;
    float posy = 140;

    float tamano_base = 16.0f;
    float tamano_actual = 16.0f;

    enum Opcion { JUGAR, OPCIONES, CONTROLES, CREDITOS };
    Opcion opcionActual = JUGAR;
};

class Menu {
public:

    Letra titulo[6] = {{0,0,-2.0},{0,0,-2.1},{0,0,-2.2},{0,0,-2.3},{0,0,-2.4},{0,0,-2.5}};
    Tractor tractor;
    Paloma paloma;
    Nube nube[2] = {{0,210,-1}, {240,210,-1.1} };
    Selector selector;

    float dt;
    float angulo{};
    double ancho = 480;
    double alto = 270;

    //enum Estado { BANDO, OPCIONES, CONTROLES, CREDITOS };
    //Estado estado_menu;

    void actualizar(float dt);
    void dibujar(Renderizador* motor);
    void moverSelector(int direccion);
    Selector::Opcion getOpcionActual() { return selector.opcionActual; }
};

