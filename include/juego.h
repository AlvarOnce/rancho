#pragma once

#include "menu.h"
#include "tablero.h"
#include "arena.h"
#include "renderizador.h" 
#include "animal.h"
#include "cabra.h"
#include "cerdo.h"
#include "gallina.h"
#include "oveja.h"
#include "granjero.h"
#include "transicion.h" 
#include "creditos.h"

class Juego 
{
    Transicion transicion;
    Menu* menu;
    Tablero* tablero;
    Arena* arena;
    Renderizador* renderizador;
    Animal* animales[18];
    Creditos* creditos;

    double ancho_ventana_ = 480;
    double alto_ventana_ = 270;

public:

    double anchoVentana() const {return ancho_ventana_;} // Para decirle al main cuánto mide la ventana que debe crear
    double altoVentana() const {return alto_ventana_;}

    enum Estado { MENU, TABLERO, BATALLA, CREDITOS };
    Estado estado_actual;
    Estado proximo_estado;

    Juego(); 
    ~Juego();

    // game loop dividido en dos fases desacopladas
    void actualizarLogica(float dt); // se llama desde OnTimer
    void renderizarGraficos(); // se llama desde OnDraw

    // coordinador de inputs de teclado
    void procesarTeclaPresionada(unsigned char key);
    void procesarTeclaLevantada(unsigned char key);
    void procesarTeclaEspecialPresionada(int key);
    void procesarTeclaEspecialLevantada(int key);
};