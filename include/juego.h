#pragma once

#include "menu.h"
#include "tablero.h"
#include "arena.h"
#include "renderizador.h" 
#include "animal.h"
#include "cabra.h"
#include "gallina.h"
#include "transicion.h" 
#include "creditos.h"
#include "controles.h"
#include "jugador.h"
#include "estructuras.h"
#include "RenderizadorAudio.h"

class Juego 
{   
    Transicion transicion_;
    Menu* menu_;
    Tablero* tablero_;
    Arena* arena_;
    RenderizadorAudio* audio_;
    Renderizador* renderizador_;
    Jugador* jugadores_[2];
    Creditos* creditos_;
    Controles* controles_;

public:

    enum Estado { MENU, TABLERO, BATALLA, CREDITOS, CONTROLES};
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