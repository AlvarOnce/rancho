#pragma once
#include "cursor.h"
#include "animal.h"

class Jugador {
private:
    int idJugador;
    int bando;

    int filaCursor;
    int colCursor;
    Cursor miCursor;

    Animal* piezaSeleccionada;
    int filaPiezaVirtual; // dónde está la pieza "volando" antes de soltarla
    int colPiezaVirtual;

public:
    Jugador(int id, int bandoAsignado, int filaInicio, int colInicio, float xPixel, float yPixel);

    // Para la fase NAVEGANDO
    void moverCursorVacio(int deltaFila, int deltaCol);
    void dibujarCursorVacio(Renderizador* motor);

    // Para la fase MOVIENDO_PIEZA
    void moverPiezaVirtual(int deltaFila, int deltaCol);
    void dibujarPiezaFlotando(Renderizador* motor);

    // Getters y Setters
    int getBando() { return bando; }
    int getFilaCursor() { return filaCursor; }
    int getColCursor() { return colCursor; }

    int getFilaPiezaVirtual() { return filaPiezaVirtual; }
    int getColPiezaVirtual() { return colPiezaVirtual; }

    Animal* getPieza() { return piezaSeleccionada; }
    bool tienePiezaAgarrada() { return piezaSeleccionada != nullptr; }

    void agarrarPieza(Animal* p) {
        piezaSeleccionada = p;
        filaPiezaVirtual = filaCursor;
        colPiezaVirtual = colCursor;
    }

    void soltarPieza() {
        piezaSeleccionada = nullptr;
    }
};