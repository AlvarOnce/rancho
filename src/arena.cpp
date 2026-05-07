#include "arena.h"
#include <cmath>
#include <cstdlib>

Arena::Arena() {
	combatientes[0] = nullptr;
	combatientes[1] = nullptr;
	ganador = -1;
	combate_terminado = false;

	for (int i = 0; i < 2; i++){
		pos_x[i] = pos_y[i] = 0;
		vivo[i] = false;
		movimiento_arriba[i] = movimiento_abajo[i] = false;
		movimiento_dch[i] = movimiento_izq[i] = false;
		disparo_disparado[i] = false;
		disparo_x[i] = disparo_y[i] = 0;
		direccion_disparo_x[i] = direccion_disparo_y[i] = 0;
		recarga_de_ataque[i] = 0;//tiempo que falta para poder atacar
		recarga_maxima_de_ataque[i] = 1.0f;//tiempo que tiene que pasar para poder atacar de nuevo.
	}
	
	contador_de_ciclo_barreras = 0;
	ciclo_maximo_barreras = 3.0f; //las baerras se mueven cada 3 seg

	for (int i = 0; i < NUM_DE_BARRERAS; i++) {
		barrera_x[i] = 0;
		barrera_y[i] = 0;
		barrera_visible[i] = true;
	}
}

Arena::~Arena() {
	//las piezas se deben de destruir en tablero.
}

void Arena::inicioCombate(Animal* pieza_luz, Animal* pieza_oscuridad)
{
	combatientes[0] = pieza_luz;
	combatientes[1] = pieza_oscuridad;

	//colocacion de las piezas en la arena
	pos_x[0] = 80.0f;  
	pos_y[0] = ALTO_DE_LA_ARENA / 2.0f;
	pos_x[1] = ANCHO_DE_LA_ARENA - 80.0f;
	pos_y[1] = ALTO_DE_LA_ARENA / 2.0f;

	vivo[0] = true;
	vivo[1] = true;

	//recargamos el ataque de cada pieza
	for (int i = 0; i < 2; i++){
		int tipo_de_ataque = 0; //combatientes[i]->obtenerTipoDeAtaque(); //cambiar para lo de la funcion ObtenerTipoDeAtaque 
		if (tipo_de_ataque == ATAQUE_TIPO_GOLPE) recarga_maxima_de_ataque[i] = 0.5f;
		if (tipo_de_ataque == ATAQUE_TIPO_DISPARO)recarga_maxima_de_ataque[i] = 1.0f;
		if (tipo_de_ataque == ATAQUE_EN_AREA) recarga_maxima_de_ataque[i] = 2.0f;
		recarga_de_ataque[i] = 0;
	}
	ganador = -1;
	combate_terminado = false;

	colocarBarrerasAleatorias();

}

void Arena::actualizar(float dt) {
	if (combate_terminado) return;

	actualizarMovimiento(dt);
	actualizarDisparo(dt);
	actualizarRecarga(dt);
	actualizarBarreras(dt);
	confirmarImpacto();
	confirmarFinCombate();
}

void Arena::dibujar(Renderizador* renderizador) {
	//code para dibujar la arena

	//dibujamos barreras aqui
	for (int i = 0; i < NUM_DE_BARRERAS; i++) {
		if (barrera_visible[i]) {
			//code de dibujar las barreras, importante, hay que dibujar tanto barrera x[] y barrera y[]
		}
	}

	for (int i = 0; i < 2; i++) {
		if (disparo_disparado[i]) {
			//los mismo que barreras, dibujar en x e y
		}
	}

	for (int i = 0; i < 2; i++) {
		if (vivo[i] && combatientes[i] != nullptr) {
			//combatientes[i]->dibujar(pos_x[i], pos_y[i]); no se porque da error
		}
	}
}

void Arena::recibirMovimiento(int jugador, int movimiento, bool tecla_pulsada) {
	if (movimiento == ARRIBA) movimiento_arriba[jugador] = true;
	if (movimiento == ABAJO) movimiento_abajo[jugador] = true;
	if (movimiento == IZQUIERDA) movimiento_izq[jugador] = true;
	if (movimiento == DERECHA) movimiento_dch[jugador] = true;
}

bool Arena::recibirAtaque(int jugador) {
	if (combate_terminado) return false;
	if (!vivo[jugador]) return false;

	if (recarga_de_ataque[jugador] > 0) return false;

	int tipo = 0; //combatientes[jugador]->getTipoAtaque();//getTipoAtaque la tendra animal, el cero es para que no de error
	
	if (tipo == ATAQUE_TIPO_DISPARO) {
		direccion_disparo_x[jugador] = ultima_direccion_x[jugador];
		direccion_disparo_y[jugador] = ultima_direccion_y[jugador];
		disparo_x[jugador] = pos_x[jugador];
		disparo_y[jugador] = pos_y[jugador];
		disparo_disparado[jugador] = true;
	}
	else if (tipo == ATAQUE_TIPO_GOLPE) {
		int rival = (jugador == 0) ? 1 : 0;
		float dx = pos_x[jugador] - pos_x[rival];
		float dy = pos_x[jugador] - pos_y[rival];
		float dist = sqrt(dx * dx + dy * dy);
		if (dist < 30.0f) {
			//combatientes[rival]->recibirDano(combatientes[jugador]->getdano());
		}
	} else if (tipo == ATAQUE_EN_AREA) {
		int rival = (jugador == 0) ? 1 : 0;
		float dx = pos_x[jugador] - pos_x[rival];
		float dy = pos_y[jugador] - pos_y[rival];
		float dist = sqrt(dx * dx + dy * dy);
		/*
		* float alcance = combatientes[jugador]->getAlcance();
		* if(dist<alcance){
		*combatientes[rival]->recibirDano(combatientes[jugador]->getDano()); 
		}
		*/
	}

	recarga_de_ataque[jugador] = recarga_maxima_de_ataque[jugador];
}

bool Arena::combateTerminado() const { return combate_terminado; }
int Arena::ganadorCombate()const { return ganador; }

void Arena::actualizarMovimiento(float dt) {
	for (int i = 0; i < 2; i++) {
		if (!vivo[i]) continue;

		float velocidad = 8; //combatientes[i]->getVelocidad(); luego hay que descomentar obvio
		if (movimiento_arriba[i])pos_y[i] -= velocidad * dt;
		if (movimiento_abajo[i])pos_y[i] += velocidad * dt;
		if (movimiento_izq[i])pos_x[i] -= velocidad * dt;
		if (movimiento_dch[i])pos_x[i] += velocidad * dt;

		mantenerLimites(i);

	}
}

void Arena::actualizarDisparo(float dt) {
	float velocidad_proyectil = 200.0f;

	for (int i = 0; i < 2; i++) {
		if (!disparo_disparado[i]) continue;

		disparo_x[i] += direccion_disparo_x[i] * velocidad_proyectil * dt;
		disparo_y[i] += direccion_disparo_y[i] * velocidad_proyectil * dt;

		// si sale de la pantalla lo desactivamos
		if (disparo_x[i] < 0 || disparo_x[i] > ANCHO_DE_LA_ARENA ||
			disparo_x[i] < 0 || disparo_y[i] > ALTO_DE_LA_ARENA) {
			disparo_disparado[i] = false;
		}

		// si choca con una barrera lo desactivamos
		if (colisionBarrera(disparo_x[i], disparo_y[i])) {
			disparo_disparado[i] = false;
		}
	}
}

void Arena::actualizarRecarga(float dt) {
	for (int i = 0; i < 2; i++) {
		if (recarga_de_ataque[i] > 0)
			recarga_de_ataque[i] -= dt;
	}
}

void Arena::actualizarBarreras(float dt) {
	// avanzamos el ciclo
	contador_de_ciclo_barreras += dt;

	if (contador_de_ciclo_barreras >= ciclo_maximo_barreras) {
		contador_de_ciclo_barreras = 0;
		for (int i = 0; i < NUM_DE_BARRERAS; i++) {
			barrera_visible[i] = !barrera_visible[i];
		}
	}
}

void Arena::confirmarImpacto() {
	for (int i = 0; i < 2; i++) {
		if (!disparo_disparado[i]) continue;

		int rival = (i == 0) ? 1 : 0;
		if (!vivo[rival]) continue;

		float dx = disparo_x[i] - pos_x[rival];
		float dy = disparo_y[i] - pos_y[rival];
		float dist = sqrt(dx * dx + dy * dy);

		if (dist < 15.0f) { // radio de impacto
			//combatientes[rival]->recibirDano(combatientes[i]->getDano());
			disparo_disparado[i] = false;
		}
	}
}

void Arena::confirmarFinCombate() {
	/*
	for (int i = 0; i < 2; i++) {
		if (combatientes[i] != nullptr && combatientes[i]->getVida() <= 0) {
			vivo[i] = false;
			combate_terminado = true;
			//ganador = (i == 0) ? BANDO_OSCURIDAD : BANDO_LUZ;
		}
	}*/
}

void Arena::mantenerLimites(int jugador) {
	if (pos_x[jugador] < 0) pos_x[jugador] = 0;
	if (pos_x[jugador] > ANCHO_DE_LA_ARENA) pos_x[jugador] = ANCHO_DE_LA_ARENA;
	if (pos_y[jugador] < 0) pos_y[jugador] = 0;
	if (pos_y[jugador] > ALTO_DE_LA_ARENA) pos_y[jugador] = ALTO_DE_LA_ARENA;
}

bool Arena::colisionBarrera(float x, float y) {
	for (int i = 0; i < NUM_DE_BARRERAS; i++) {
		if (!barrera_visible[i]) continue;
		float dx = x - barrera_x[i];
		float dy = y - barrera_y[i];
		if (dx > -20 && dx < 20 && dy > -20 && dy < 20)
			return true;
	}
	return false;
}

void Arena::colocarBarrerasAleatorias() {
	for (int i = 0; i < NUM_DE_BARRERAS; i++) {
		barrera_x[i] = 100.0f + (rand() % (ANCHO_DE_LA_ARENA - 200));
		barrera_y[i] = 40.0f + (rand() % (ALTO_DE_LA_ARENA - 80));
		barrera_visible[i] = true;
	}
}
