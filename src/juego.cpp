#include <iostream>
#include <stdlib.h>
#include "arena.h"
#include "juego.h"

Juego::Juego() 
{
    estado_actual = MENU;
    proximo_estado = MENU;

    menu_ = new Menu();
    arena_ = new Arena();
    renderizador_ = new Renderizador();
    creditos_ = new Creditos();
    controles_ = new Controles();
    ganador_ = new Ganador();
    audio_ = new RenderizadorAudio();

    for (int i = 0; i < 2; i++)
        jugadores_[i] = new Jugador(i);
    tablero_ = new Tablero(jugadores_[0], jugadores_[1]);
    audio_->sonar(menu_);
}

Juego::~Juego() 
{
    delete menu_;
    delete tablero_;
    delete arena_;
    delete renderizador_;
	delete creditos_;
    delete controles_;
    delete ganador_;
    for (int i = 0; i < 2; i++)
        delete jugadores_[i];
}

void Juego::actualizarLogica(float dt) // FASE 1: matemáticas, colisiones y reglas del juego
{
    switch (estado_actual)
    {
    case MENU:
        menu_->actualizar(dt);
        break;

    case TABLERO:
        tablero_->actualizar(dt);
        if (tablero_->enBatalla)
        {
            transicion_.empieza();
            proximo_estado = BATALLA;
            tablero_->enBatalla = false;

            arena_->setCombatientes(jugadores_[0]->getAnimalEnCombate(), jugadores_[1]->getAnimalEnCombate());
        }

        if (tablero_->determinarGanador() != -1)
        {
            transicion_.empieza();
            proximo_estado = GANADOR;
        }
        break;

    case BATALLA:

        arena_->actualizar(dt);

        if (arena_->combateTerminado())
        {
            Animal* animalPerdedor = jugadores_[arena_->obtenerPerdedor()]->getAnimalEnCombate();
            Animal* animalGanador = jugadores_[1 - arena_->obtenerPerdedor()]->getAnimalEnCombate();

            tablero_->acomodarGanador(animalGanador);

            animalPerdedor->vida_ = 0;
            animalPerdedor->setPosicion(Vector2D(-100, -100));

            transicion_.empieza();
            proximo_estado = TABLERO;
        }
        break;

    case CREDITOS:
        if (!transicion_.getActivo())
            creditos_->actualizar(25);
        if (creditos_->getFinalizado())
        {
            transicion_.empieza();
            proximo_estado = MENU;
        }
        break;

    case CONTROLES:
        if (!transicion_.getActivo())
            controles_->actualizar(25);
        if (controles_->getFinalizado())
        {
            transicion_.empieza();
            proximo_estado = MENU;
        }
        break;

    case GANADOR:


        break;
    }

    if (transicion_.getActivo())
        transicion_.actualizar(dt);

    if (transicion_.getEstado() == Transicion::CERRADO)
    {
        estado_actual = proximo_estado; // solo aqui, una sola vez
        switch (estado_actual)
        {
        case MENU:     audio_->sonar(menu_);    break;
        case TABLERO:  audio_->sonar(tablero_); break;
        case BATALLA:  audio_->sonar(arena_);   break;
        case CREDITOS: audio_->sonar(creditos_); break;
        case CONTROLES: audio_->sonar(controles_); break;
        }
    }
}

void Juego::renderizarGraficos() // FASE 2: pintar en pantalla
{   
    renderizador_->limpiarPantalla();

    switch (estado_actual)
    {
    case MENU:
        renderizador_->dibujar(menu_);
        break;

    case TABLERO:
        renderizador_->dibujar(tablero_);
        break;

    case BATALLA:
        renderizador_->dibujar(arena_);
        break;

    case CREDITOS:
        renderizador_->dibujar(creditos_);
        break;

    case CONTROLES:
        renderizador_->dibujar(controles_);
        break;

    case GANADOR:
        renderizador_->dibujar(ganador_);
        break;
    }

    renderizador_->dibujar(&transicion_);
}

void Juego::procesarTeclaPresionada(unsigned char key) // Hacer que tecla solo se procese si transicion.activo = false
{
    if (key == 27) exit(0); // Esc siempre cierra el juego, aunque en un futuro molaría poner un menú de pausa

	if (key == 'b' || key == 'B') // temporalmente, para saltar el menú y probar la batalla directamente
    {
        transicion_.empieza();
        proximo_estado = BATALLA;
        arena_->setCombatientes(jugadores_[0]->getAnimalEnCombate(), jugadores_[1]->getAnimalEnCombate());
        arena_->inicioCombate();
        return;
    }

    switch (estado_actual) 
    {
        case MENU:

        if (key == 13) { // Intro para elegir una opción

            switch (menu_->getOpcionActual()) {

            case Selector::JUGAR:
                audio_->stopMusica();
                audio_->sonarTransicion();
                transicion_.empieza();
                proximo_estado = TABLERO;
                break;

            //case Selector::OPCIONES: // en opciones puede estar el volumen o quizá algo del juego
            //    estado_actual = OPCIONES; // no sé si los submenús del ménu son un estado 
            //    break;

            case Selector::CREDITOS:
                creditos_->reset();
                transicion_.empieza();
                proximo_estado = CREDITOS;
                break;

            case Selector::CONTROLES:
                controles_->reset();
                transicion_.empieza();
                proximo_estado = CONTROLES;
                break;
            }
        }

        if (key == 'b') {
            transicion_.empieza();
            proximo_estado = BATALLA;
        }
        break;

		case TABLERO: // movimiento discreto en el tablero, no hace falta procesar la tecla al levantarla, el movimiento se hace una vez al pulsar y ya está
         
         if (key == 'w' || key == 'W') tablero_->recibirMovimiento(0, 0, 1); // tablero->recibirMovimiento(jugador, dx, dy);
         if (key == 's' || key == 'S') tablero_->recibirMovimiento(0, 0, -1);
         if (key == 'a' || key == 'A') tablero_->recibirMovimiento(0, -1, 0);
         if (key == 'd' || key == 'D') tablero_->recibirMovimiento(0, 1, 0);
		 if (key == 'q' || key == 'Q') tablero_->seleccionarPieza(0,audio_); // Selección para J1
         if (key == 'm' || key == 'M') tablero_->seleccionarPieza(1,audio_); // Selección para J2
        break;

		case BATALLA: // movimiento continuo en la batalla, se procesa al pulsar la tecla y al levantarla, hay movimiento mientras se mantenga pulsada la tecla
         if (key == 'w' || key == 'W') arena_->recibirMovimiento(0, ARRIBA, true);
         if (key == 's' || key == 'S') arena_->recibirMovimiento(0, ABAJO, true);
         if (key == 'a' || key == 'A') arena_->recibirMovimiento(0, IZQUIERDA, true);
         if (key == 'd' || key == 'D') arena_->recibirMovimiento(0, DERECHA, true);                
		 if (key == 'q' || key == 'Q') arena_->recibirAtaque(0); // Ataque para J1
         if (key == 'm' || key == 'M') arena_->recibirAtaque(1); // Ataque para J2

         if (key == 'b') {
             transicion_.empieza();
             proximo_estado = MENU;
         }
         break;
    }
}

void Juego::procesarTeclaLevantada(unsigned char key)
{
    switch (estado_actual) 
    {
    case TABLERO:
        // como en el tablero el movimiento es discreto, no hay que hacer nada
        // cuando se levanta la tecla, ya se ha hecho todo al pulsarla.
        break;

    case BATALLA:
        if (key == 'w' || key == 'W') arena_->recibirMovimiento(0, ARRIBA, false);
        if (key == 's' || key == 'S') arena_->recibirMovimiento(0, ABAJO, false);
        if (key == 'a' || key == 'A') arena_->recibirMovimiento(0, IZQUIERDA, false);
        if (key == 'd' || key == 'D') arena_->recibirMovimiento(0, DERECHA, false);
        break;
    }
}

void Juego::procesarTeclaEspecialPresionada(int key) // JUGADOR 2 (FLECHAS)
{
    switch (estado_actual) 
    {
    case MENU:
        if (key == GLUT_KEY_UP) menu_->moverSelector(-1); // arriba resta 1 (se acerca a 0 que es JUGAR)
        if (key == GLUT_KEY_DOWN) menu_->moverSelector(1); // abajo suma 1 (bajándo hacia el 3 que es CREDITOS)
        break;

    case TABLERO:
        if (key == GLUT_KEY_UP)    tablero_->recibirMovimiento(1, 0, 1); // tablero->recibirMovimiento(jugador, dx, dy);
        if (key == GLUT_KEY_DOWN)  tablero_->recibirMovimiento(1, 0, -1);
        if (key == GLUT_KEY_LEFT)  tablero_->recibirMovimiento(1, -1, 0);
        if (key == GLUT_KEY_RIGHT) tablero_->recibirMovimiento(1, 1, 0);
        break;

    case BATALLA:
        if (key == GLUT_KEY_UP)    arena_->recibirMovimiento(1, ARRIBA, true);
        if (key == GLUT_KEY_DOWN)  arena_->recibirMovimiento(1, ABAJO, true);
        if (key == GLUT_KEY_LEFT)  arena_->recibirMovimiento(1, IZQUIERDA, true);
        if (key == GLUT_KEY_RIGHT) arena_->recibirMovimiento(1, DERECHA, true);
        break;
    }
}

void Juego::procesarTeclaEspecialLevantada(int key)
{
    switch (estado_actual) 
    {
    case TABLERO:
        // igual que antes, aquí no hay que poner nada
        break;

    case BATALLA:
        if (key == GLUT_KEY_UP)    arena_->recibirMovimiento(1, ARRIBA, false);
        if (key == GLUT_KEY_DOWN)  arena_->recibirMovimiento(1, ABAJO, false);
        if (key == GLUT_KEY_LEFT)  arena_->recibirMovimiento(1, IZQUIERDA, false);
        if (key == GLUT_KEY_RIGHT) arena_->recibirMovimiento(1, DERECHA, false);
        break;
    }
}
