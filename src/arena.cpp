#include "arena.h"
#include <cmath>
#include <cstdlib>
#include <ctime>

const float Arena::DURACION_ATAQUE = 0.2f;
Arena::Arena() {
	srand(time(nullptr));
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
		ataque_activo[i] = false;
		ataque_x[i] = 0;
		ataque_y[i] = 0;
		ataque_visible_tiempo[i] = 0;
	}

	for (int i = 0; i < NUM_DE_BARRERAS; i++) {
		barrera_x[i] = 0;
		barrera_y[i] = 0;
		ciclo_maximo_barrera[i] = 3000.0f + (rand() % 8000);
		barrera_visible[i] = (rand() % 2 == 0);
		contador_ciclo_barrera[i] = (float)(rand() % (int)ciclo_maximo_barrera[i]);
	}
}

Arena::~Arena() {
	//las piezas se deben de destruir en tablero.
}

void Arena::inicioCombate(Animal* pieza_luz, Animal* pieza_oscuridad)
{
	combatientes[0] = pieza_luz;
	combatientes[1] = pieza_oscuridad;

	// desactivamos toda la logica del tablero para estos animales
	combatientes[0]->intro_tablero_ = false;
	combatientes[0]->en_movimiento_ = false;
	combatientes[0]->casillas_movidas_ = 0;
	combatientes[0]->casillas_movidas_x_ = 0;
	combatientes[0]->casillas_movidas_y_ = 0;
	combatientes[0]->velx_ = 0;
	combatientes[0]->vely_ = 0;

	combatientes[1]->intro_tablero_ = false;
	combatientes[1]->en_movimiento_ = false;
	combatientes[1]->casillas_movidas_ = 0;
	combatientes[1]->casillas_movidas_x_ = 0;
	combatientes[1]->casillas_movidas_y_ = 0;
	combatientes[1]->velx_ = 0;
	combatientes[1]->vely_ = 0;

	// colocacion en la arena
	pos_x[0] = ARENA_MARGEN_X + 30.0f;
	pos_y[0] = ARENA_MARGEN_Y + ZONA_DE_COMBATE_Y / 2.0f;
	pos_x[1] = ARENA_MARGEN_X + ZONA_DE_COMBATE_X - 30.0f;
	pos_y[1] = ARENA_MARGEN_Y + ZONA_DE_COMBATE_Y / 2.0f;

	combatientes[0]->posx_ = pos_x[0];
	combatientes[0]->posy_ = pos_y[0];
	combatientes[1]->posx_ = pos_x[1];
	combatientes[1]->posy_ = pos_y[1];

	vivo[0] = true;
	vivo[1] = true;

	for (int i = 0; i < 2; i++) {
		int tipo_de_ataque = ATAQUE_TIPO_GOLPE;
		if (tipo_de_ataque == ATAQUE_TIPO_GOLPE)   recarga_maxima_de_ataque[i] = 0.5f;
		if (tipo_de_ataque == ATAQUE_TIPO_DISPARO) recarga_maxima_de_ataque[i] = 1.0f;
		if (tipo_de_ataque == ATAQUE_EN_AREA)      recarga_maxima_de_ataque[i] = 2.0f;
		recarga_de_ataque[i] = 0;
	}

	ganador = -1;
	combate_terminado = false;

	colocarBarrerasAleatorias();
}

void Arena::actualizar(float dt) {
	if (combate_terminado) return;
	actualizarBarreras(dt);
	actualizarMovimiento(dt);
	actualizarDisparo(dt);
	actualizarAtaque(dt);
	actualizarRecarga(dt);
	confirmarImpacto();
	confirmarFinCombate();
}

void Arena::dibujar(Renderizador* renderizador)
{
	renderizador->dibujarArena(ARENA_MARGEN_X, ARENA_MARGEN_Y, ZONA_DE_COMBATE_X, ZONA_DE_COMBATE_Y, 0.1f, 0.2f, 0.6f, -5.0f);
	//dibujamos torres con rectangulos

	for (int i = 0; i < NUM_DE_BARRERAS; i++)
	{
		if (barrera_visible[i])
		{
			renderizador->dibujarBarreras(barrera_x[i] - 7, barrera_y[i] - 9, 14, 18, 0.6f, 0.6f, 0.6f, -3.0f);
			renderizador->dibujarBarreras(barrera_x[i] - 8, barrera_y[i] - 11, 16, 4, 0.8f, 0.8f, 0.8f, 0.1f);
		}
		else
		{
			renderizador->dibujarBarreras(barrera_x[i] - 6, barrera_y[i] - 11, 12, 3, 0.2f, 0.2f, 0.2f, -0.9f);
		}
	}

	for (int i = 0; i < 2; i++) {
		if (ataque_activo[i]) {
			renderizador->dibujarBarreras(ataque_x[i] - 4, ataque_y[i] - 8, 8, 16, 0.55f, 0.27f, 0.07f, -2.0f);
		}
	}

	// gallinas
	glDisable(GL_DEPTH_TEST);
	for (int i = 0; i < 2; i++) {
		if (vivo[i] && combatientes[i] != nullptr) {
			renderizador->dibujarSprite(
				"../assets/Sprites/gallina/gallinaSpritesheet.png",
				256, 32,  // doble de tamaño solo en la arena
				combatientes[i]->posx_,
				combatientes[i]->posy_,
				-4.5f,
				1, 8,
				combatientes[i]->frameActualX_,
				combatientes[i]->frameActualY_,
				true
			);
		}
	}
	glEnable(GL_DEPTH_TEST);
}


void Arena::recibirMovimiento(int jugador, int movimiento, bool tecla_pulsada) {
	if (movimiento == ARRIBA)    movimiento_arriba[jugador] = tecla_pulsada;
	if (movimiento == ABAJO)     movimiento_abajo[jugador] = tecla_pulsada;
	if (movimiento == IZQUIERDA) movimiento_izq[jugador] = tecla_pulsada;
	if (movimiento == DERECHA)   movimiento_dch[jugador] = tecla_pulsada;
}

bool Arena::recibirAtaque(int jugador) {
	if (combate_terminado) return false;
	if (!vivo[jugador]) return false;
	if (recarga_de_ataque[jugador] > 0) return false;

	int tipo = ATAQUE_TIPO_GOLPE; //combatientes[jugador]->getTipoAtaque();

	if (tipo == ATAQUE_TIPO_DISPARO) {
		direccion_disparo_x[jugador] = ultima_direccion_x[jugador];
		direccion_disparo_y[jugador] = ultima_direccion_y[jugador];
		disparo_x[jugador] = pos_x[jugador];
		disparo_y[jugador] = pos_y[jugador];
		disparo_disparado[jugador] = true;

	}
	else if (tipo == ATAQUE_TIPO_GOLPE) {
		int rival = (jugador == 0) ? 1 : 0;

		// colocamos el palo delante de la gallina segun donde mire
		float offset = 18.0f;
		ataque_x[jugador] = pos_x[jugador] + ultima_direccion_x[jugador] * offset;
		ataque_y[jugador] = pos_y[jugador] + ultima_direccion_y[jugador] * offset;
		ataque_activo[jugador] = true;
		ataque_visible_tiempo[jugador] = 0;

		// comprobamos si el rival esta en rango de golpe
		float dx = pos_x[jugador] - pos_x[rival];
		float dy = pos_y[jugador] - pos_y[rival];
		float dist = sqrt(dx * dx + dy * dy);

		if (dist < 30.0f) {
			combatientes[rival]->vida_ -= combatientes[jugador]->ataque_;
			std::cout << "Jugador " << rival + 1 << " recibe golpe. Vida restante: "
				<< combatientes[rival]->vida_ << std::endl;
		}

	}
	else if (tipo == ATAQUE_EN_AREA) {
		int rival = (jugador == 0) ? 1 : 0;
		float dx = pos_x[jugador] - pos_x[rival];
		float dy = pos_y[jugador] - pos_y[rival];
		float dist = sqrt(dx * dx + dy * dy);
		/*
		* float alcance = combatientes[jugador]->getAlcance();
		* if(dist<alcance){
		*   combatientes[rival]->recibirDano(combatientes[jugador]->getDano());
		* }
		*/
	}

	recarga_de_ataque[jugador] = recarga_maxima_de_ataque[jugador];
	return true;
}

bool Arena::combateTerminado() const { return combate_terminado; }
int Arena::ganadorCombate()const { return ganador; }

void Arena::actualizarMovimiento(float dt)
{
	for (int i = 0; i < 2; i++)
	{
		if (!vivo[i]) continue;

		pos_antigua_x[i] = pos_x[i];
		pos_antigua_y[i] = pos_y[i];

		float velocidad = 100.0f;
		float dt_seg = dt / 1000.0f;  // convertimos a segundos

		if (movimiento_arriba[i]) pos_y[i] += velocidad * dt_seg;
		if (movimiento_abajo[i])  pos_y[i] -= velocidad * dt_seg;
		if (movimiento_izq[i])    pos_x[i] -= velocidad * dt_seg;
		if (movimiento_dch[i])    pos_x[i] += velocidad * dt_seg;

		if (movimiento_arriba[i]) { ultima_direccion_x[i] = 0; ultima_direccion_y[i] = 1; }
		if (movimiento_abajo[i]) { ultima_direccion_x[i] = 0; ultima_direccion_y[i] = -1; }
		if (movimiento_izq[i]) { ultima_direccion_x[i] = -1; ultima_direccion_y[i] = 0; }
		if (movimiento_dch[i]) { ultima_direccion_x[i] = 1; ultima_direccion_y[i] = 0; }

		if (colisionBarrera(pos_x[i], pos_y[i]))
		{
			pos_x[i] = pos_antigua_x[i];
			pos_y[i] = pos_antigua_y[i];
		}

	
		int rival = (i == 0) ? 1 : 0;
		float dx = pos_x[i] - pos_x[rival];
		float dy = pos_y[i] - pos_y[rival];
		float dist = sqrt(dx * dx + dy * dy);
		if (dist < 22.0f) 
		{
			pos_x[i] = pos_antigua_x[i];
			pos_y[i] = pos_antigua_y[i];
		}
		mantenerLimites(i);

		if (combatientes[i] != nullptr) {
			combatientes[i]->posx_ = pos_x[i];
			combatientes[i]->posy_ = pos_y[i];
		}
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
			recarga_de_ataque[i] -= dt/1000;
	}
}

void Arena::actualizarBarreras(float dt) {
	for (int i = 0; i < NUM_DE_BARRERAS; i++) {
		contador_ciclo_barrera[i] += dt;
		if (contador_ciclo_barrera[i] >= ciclo_maximo_barrera[i]) 
		{
			contador_ciclo_barrera[i] = 0.0f;
			barrera_visible[i] = !barrera_visible[i];
			ciclo_maximo_barrera[i] = 10000.0f + (rand() % 10);

			if (barrera_visible[i]) 
			{
				for (int j = 0; j < 2; j++) 
				{
					if (!vivo[j]) continue;
					float dx = pos_x[j] - barrera_x[i];
					float dy = pos_y[j] - barrera_y[i];
					if (dx > -10 && dx < 10 && dy > -12 && dy < 12) 
					{
						// empujamos al jugador fuera de la barrera
						// lo movemos hacia el lado mas cercano
						if (abs(dx) > abs(dy))
							pos_x[j] += (dx > 0) ? 12.0f : -12.0f;
						else
							pos_y[j] += (dy > 0) ? 14.0f : -14.0f;

						// sincronizamos con el animal
						if (combatientes[j] != nullptr) 
						{
							combatientes[j]->posx_ = pos_x[j];
							combatientes[j]->posy_ = pos_y[j];
						}
					}
				}
			}
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

void Arena::confirmarFinCombate() 
{
	for (int i = 0; i < 2; i++) {
		if (combatientes[i] != nullptr && combatientes[i]->vida_ <= 0) {
			vivo[i] = false;
			combate_terminado = true;
			ganador = (i == 0) ? 1 : 0;
			std::cout << "Jugador " << ganador + 1 << " gana el combate!" << std::endl;
		}
	}
}

void Arena::mantenerLimites(int jugador)
{
	float radio = 11.0f;
	float antes_x = pos_x[jugador];
	float antes_y = pos_y[jugador];

	if (pos_x[jugador] < ARENA_MARGEN_X + radio) pos_x[jugador] = ARENA_MARGEN_X + radio;
	if (pos_x[jugador] > ARENA_MARGEN_X + ZONA_DE_COMBATE_X - radio) pos_x[jugador] = ARENA_MARGEN_X + ZONA_DE_COMBATE_X - radio;
	if (pos_y[jugador] < ARENA_MARGEN_Y + radio) pos_y[jugador] = ARENA_MARGEN_Y + radio;
	if (pos_y[jugador] > ARENA_MARGEN_Y + ZONA_DE_COMBATE_Y - radio) pos_y[jugador] = ARENA_MARGEN_Y + ZONA_DE_COMBATE_Y - radio;

	if (antes_x != pos_x[jugador] || antes_y != pos_y[jugador])
		std::cout << "LIMITE ACTIVADO J" << jugador << ": de (" << antes_x << "," << antes_y << ") a (" << pos_x[jugador] << "," << pos_y[jugador] << ")" << std::endl;
}

bool Arena::colisionBarrera(float x, float y) {
	for (int i = 0; i < NUM_DE_BARRERAS; i++) 
	{
		if (!barrera_visible[i]) continue;
		float dx = x - barrera_x[i];
		float dy = y - barrera_y[i];
		if (dx > -10 && dx < 10 && dy > -12 && dy < 12)
			return true;
	}
	return false;
}

void Arena::colocarBarrerasAleatorias() {
	float margen = 15.0f;
	float zona_x = (ZONA_DE_COMBATE_X - margen * 2) / 4.0f;
	float zona_y = (ZONA_DE_COMBATE_Y - margen * 2) / 2.0f;

	for (int i = 0; i < NUM_DE_BARRERAS; i++) {
		int columna = i % 4;
		int fila = i / 4;

		// esquina de cada zona dentro del cuadrado azul
		float origen_x = ARENA_MARGEN_X + margen + (columna * zona_x);
		float origen_y = ARENA_MARGEN_Y + margen + (fila * zona_y);

		// posicion aleatoria dentro de esa zona
		barrera_x[i] = origen_x + (rand() % (int)(zona_x * 0.7f));
		barrera_y[i] = origen_y + (rand() % (int)(zona_y * 0.7f));

	}
}

void Arena::actualizarAtaque(float dt)
{
	for (int i = 0; i < 2; i++) {
		if (!ataque_activo[i]) continue;

		ataque_visible_tiempo[i] += dt/1000;

		// el palo desaparece despues de DURACION_ATAQUE segundos
		if (ataque_visible_tiempo[i] >= DURACION_ATAQUE)
		{
			ataque_activo[i] = false;
			ataque_visible_tiempo[i] = 0;
		}
	}
}
	
int Arena::obtenerPerdedor() const {
	return (ganador == 0) ? 1 : 0;
}

