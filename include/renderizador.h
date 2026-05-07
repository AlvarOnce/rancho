#pragma once

// este es el ÚNICO archivo .h de todo el proyecto que debe incluir librerías gráficas. (leer a continuación)
// animal.h necesita ETSIDI.h para crear un puntero a SpriteSequence, pero se mantiene cierto para freeglut.h

#include "freeglut.h"
#include "ETSIDI.h"

class Renderizador {
public:

    void limpiarPantalla();

    void dibujarSprite(
		const char* rutaImagen, // ruta del sprite a dibujar
		float ancho, // ancho y alto del sprite, si es un sprite sheet, el ancho y alto total del sprite sheet, no del frame individual
        float alto, 
		float posx, // posición del centro del sprite en la ventana, no de la esquina superior izquierda
        float posy, 
		float capa, // la capa en la que se dibuja el sprite, cuanto mayor sea el número, más cerca del espectador estará el sprite (capa = profundidad)
		int rows = 1, // número de filas y columnas del sprite sheet, si el sprite no tiene animación, se deja en 1
        int cols = 1, 
		int stateX = 0, // número de frame en la dirección horizontal (columna), empezando por 0
        int stateY = 0, 
		bool transparencia = true	// si el sprite tiene transparencia, se activa el alpha test para no dibujar los píxeles completamente transparentes, 
									// esto mejora rendimiento y evita artefactos raros entre animales
    );

};