#include <iostream>
#include <stdlib.h>
#include "arena.h"
#include "juego.h"

#define numeroAnimales 18 // Por ahora crea 18 entidades

Juego::Juego() {

    estado_actual = MENU;
    proximo_estado = MENU;

    menu = new Menu();
    arena = new Arena();
    renderizador = new Renderizador();
    creditos = new Creditos();
    controles = new Controles();

     // Animales equipo 1, lado izquierdo
        for (int i = 0; i < numeroAnimales; i++) // se crean de arriba a abajo 
        {
            if (i < 9) 
            animalesJ1[i] = new Oveja(-44 - 15 * (i) + 11, 36 + 176 - (22 * i) + 11, -3 - 0.01 * i - 0, 20, 152, 0); // dejar los numeros asi por ahora porque representan unidades conocidas

            if (i >= 9)
            animalesJ1[i] = new Gallina( - 15 * (i-9) + 11, 36 + 176 - (22 * (i-9)) + 11, -3 - 0.01 * (i-9) - 0.08, 20, 152+22, 0);
           // animalesJ1[i + j * numeroAnimales] = new Cerdo(-44 * j - 15 * (numeroAnimales - i) + 11, 36 + (22 * i) + 11, -3 - 0.01 * i - 0.08 * j, 20, 152);
        }

     // Animales equipo 2, lado derecho
        for (int i = 0; i < numeroAnimales; i++)
        {
            if (i < 9)
                animalesJ2[i] = new Oveja(anchoVentana() + 44 + 15 * (i)-11, 36 + 176 - (22 * i) + 11, -3 - 0.01 * i - 0, 20, 152 + 22 * 8, 1);

            if (i >= 9)
                animalesJ2[i] = new Gallina(anchoVentana() + 15 * (i - 9) - 11, 36 + 176 - (22 * (i - 9)) + 11, -3 - 0.01 * (i - 9) - 0.08, 20, 152 + 22 * 7, 1);
            // animalesJ1[i + j * numeroAnimales] = new Cerdo(-44 * j - 15 * (numeroAnimales - i) + 11, 36 + (22 * i) + 11, -3 - 0.01 * i - 0.08 * j, 20, 152);
        }

    tablero = new Tablero(animalesJ1, animalesJ2);
}

Juego::~Juego() {

    delete menu;
    delete tablero;
    delete arena;
    delete renderizador;
	delete creditos;
    delete controles;
    for (int i = 0; i < numeroAnimales; i++)
		delete animalesJ1[i];
}

void Juego::actualizarLogica(float dt) {    // FASE 1: matemáticas, colisiones y reglas del juego
    
    switch (estado_actual) {

    case MENU:
        menu->actualizar(dt);
        break;

    case TABLERO:
        tablero->actualizar(dt);
        break;

    case BATALLA:
        arena->actualizar(dt);
        break;

    case CREDITOS:

        if (!transicion.activo)
            creditos->actualizar(25);

        if (creditos->getFinalizado())
        {
            transicion.empieza();
            proximo_estado = MENU;
        }
        break;

    case CONTROLES:

        if (!transicion.activo)
            controles->actualizar(25);
        if (controles->getFinalizado())
        {
            transicion.empieza();
            proximo_estado = MENU;
        }
        break;
    }

    if (transicion.activo)
        transicion.actualizar(dt);

    if (transicion.getEstado() == Transicion::CERRADO)
        estado_actual = proximo_estado;
}

void Juego::renderizarGraficos() {          // FASE 2: pintar en pantalla
    
    renderizador->limpiarPantalla();

    switch (estado_actual) {

    case MENU:
        menu->dibujar(renderizador);
        break;

    case TABLERO:  
        tablero->dibujar(renderizador);
        break;

    case BATALLA:
        arena->dibujar(renderizador);  
        break;

    case CREDITOS:

        creditos->dibujar(renderizador);
        break;

    case CONTROLES:

        controles->dibujar(renderizador);
        break;
    }

    if (transicion.activo) transicion.dibujar(renderizador);
}

void Juego::procesarTeclaPresionada(unsigned char key) // Hacer que tecla solo se procese si transicion.activo = false
{
    if (key == 27) exit(0); // Esc siempre cierra el juego, aunque en un futuro molaría poner un menú de pausa

    switch (estado_actual) {

        case MENU:

        if (key == 13) { // Intro para elegir una opción

            switch (menu->getOpcionActual()) {

            case Selector::JUGAR: 
                transicion.empieza();
                proximo_estado = TABLERO;
                break;

            //case Selector::OPCIONES: // en opciones puede estar el volumen o quizá algo del juego
            //    estado_actual = OPCIONES; // no sé si los submenús del ménu son un estado 
            //    break;

            //case Selector::CONTROLES: // en principio muestra nuestros controles, quizá se puedan elegir los tuyos propios
            //    estado_actual = CONTROLES;
            //    break;

            case Selector::CREDITOS:
                creditos->reset();
                transicion.empieza();
                proximo_estado = CREDITOS;
                break;

            case Selector::CONTROLES:
                controles->reset();
                transicion.empieza();
                proximo_estado = CONTROLES;
                break;
            }

        }

        break;

		case TABLERO: // movimiento discreto en el tablero, no hace falta procesar la tecla al levantarla, el movimiento se hace una vez al pulsar y ya está
         
         if (key == 'w' || key == 'W') tablero->recibirMovimiento(0, 0, 1); // tablero->recibirMovimiento(jugador, dx, dy);
         if (key == 's' || key == 'S') tablero->recibirMovimiento(0, 0, -1);
         if (key == 'a' || key == 'A') tablero->recibirMovimiento(0, -1, 0);
         if (key == 'd' || key == 'D') tablero->recibirMovimiento(0, 1, 0);
         if (key == 'q' || key == 'Q') tablero->seleccionarPieza(0);

         if (key == 'm' || key == 'M') tablero->seleccionarPieza(1); // Selección para J2

        break;

		case BATALLA: // movimiento continuo en la batalla, se procesa al pulsar la tecla y al levantarla, hay movimiento mientras se mantenga pulsada la tecla
         if (key == 'w' || key == 'W') arena->recibirMovimiento(0, ARRIBA, true);
         if (key == 's' || key == 'S') arena->recibirMovimiento(0, ABAJO, true);
         if (key == 'a' || key == 'A') arena->recibirMovimiento(0, IZQUIERDA, true);
         if (key == 'd' || key == 'D') arena->recibirMovimiento(0, DERECHA, true);
         if (key == 'q' || key == 'Q') arena->recibirAtaque(0); 

         if (key == 'm' || key == 'M') arena->recibirAtaque(1); // Ataque para J2

         break;

    }
}

void Juego::procesarTeclaLevantada(unsigned char key)
{
    switch (estado_actual) {
    case TABLERO:
        // como en el tablero el movimiento es discreto, no hay que hacer nada
        // cuando se levanta la tecla, ya se ha hecho todo al pulsarla.
        break;

    case BATALLA:
        if (key == 'w' || key == 'W') arena->recibirMovimiento(0, ARRIBA, false);
        if (key == 's' || key == 'S') arena->recibirMovimiento(0, ABAJO, false);
        if (key == 'a' || key == 'A') arena->recibirMovimiento(0, IZQUIERDA, false);
        if (key == 'd' || key == 'D') arena->recibirMovimiento(0, DERECHA, false);
        break;

    }
}

void Juego::procesarTeclaEspecialPresionada(int key) // JUGADOR 2 (FLECHAS)
{
    switch (estado_actual) {
    case MENU:
        if (key == GLUT_KEY_UP) menu->moverSelector(-1); // arriba resta 1 (se acerca a 0 que es JUGAR)
        if (key == GLUT_KEY_DOWN) menu->moverSelector(1); // abajo suma 1 (bajándo hacia el 3 que es CREDITOS)
        break;

    case TABLERO:
        if (key == GLUT_KEY_UP)    tablero->recibirMovimiento(1, 0, 1); // tablero->recibirMovimiento(jugador, dx, dy);
        if (key == GLUT_KEY_DOWN)  tablero->recibirMovimiento(1, 0, -1);
        if (key == GLUT_KEY_LEFT)  tablero->recibirMovimiento(1, -1, 0);
        if (key == GLUT_KEY_RIGHT) tablero->recibirMovimiento(1, 1, 0);
        //if (key == '.') tablero->seleccionarPieza(1); La selección del J2 va arriba, no es tecla especial
        break;

    case BATALLA:
        if (key == GLUT_KEY_UP)    arena->recibirMovimiento(1, ARRIBA, true);
        if (key == GLUT_KEY_DOWN)  arena->recibirMovimiento(1, ABAJO, true);
        if (key == GLUT_KEY_LEFT)  arena->recibirMovimiento(1, IZQUIERDA, true);
        if (key == GLUT_KEY_RIGHT) arena->recibirMovimiento(1, DERECHA, true);
        //if (key == '.') arena->recibirAtaque(1); La selección del J2 va arriba, no es tecla especial
        break;
    }
}

void Juego::procesarTeclaEspecialLevantada(int key)
{
    switch (estado_actual) {
    case TABLERO:
        // igual que antes, aquí no hay que poner nada
        break;

    case BATALLA:
        if (key == GLUT_KEY_UP)    arena->recibirMovimiento(1, ARRIBA, false);
        if (key == GLUT_KEY_DOWN)  arena->recibirMovimiento(1, ABAJO, false);
        if (key == GLUT_KEY_LEFT)  arena->recibirMovimiento(1, IZQUIERDA, false);
        if (key == GLUT_KEY_RIGHT) arena->recibirMovimiento(1, DERECHA, false);
        break;
    }
}
