#include "arena.h"
#include <cmath>
#include <cstdlib>
#include <ctime>

Arena::Arena() 
{
	srand(time(nullptr));
	combatientes_[0] = nullptr;
	combatientes_[1] = nullptr;
	ganador_ = -1;
	combate_terminado_ = false;

	for (int i = 0; i < 2; i++){
		pos_x_[i] = pos_y_[i] = 0;
		vivo_[i] = false;
		movimiento_arriba_[i] = movimiento_abajo_[i] = false;
		movimiento_dch_[i] = movimiento_izq_[i] = false;
		disparo_disparado_[i] = false;
		disparo_x_[i] = disparo_y_[i] = 0;
		direccion_disparo_x_[i] = direccion_disparo_y_[i] = 0;
		recarga_de_ataque_[i] = 0;//tiempo que falta para poder atacar
		recarga_maxima_de_ataque_[i] = 1.0f;//tiempo que tiene que pasar para poder atacar de nuevo.
		ataque_activo_[i] = false;
		ataque_x_[i] = 0;
		ataque_y_[i] = 0;
		ataque_visible_tiempo_[i] = 0;
	}

	for (int i = 0; i < NUM_DE_BARRERAS; i++) {
		barrera_x_[i] = 0;
		barrera_y_[i] = 0;
		ciclo_maximo_barrera_[i] = 3000.0f + (rand() % 8000);
		barrera_visible_[i] = (rand() % 2 == 0);
		contador_ciclo_barrera_[i] = (float)(rand() % (int)ciclo_maximo_barrera_[i]);
	}
}

Arena::~Arena() 
{
	//las piezas se deben de destruir en tablero.
}

void Arena::inicioCombate(Animal* pieza_luz, Animal* pieza_oscuridad)
{
	combatientes_[0] = pieza_luz;
	combatientes_[1] = pieza_oscuridad;

	// desactivamos toda la logica del tablero para estos animales
	for (int i = 0; i < 2; i++) {
		combatientes_[i]->intro_tablero_ = false;
		combatientes_[i]->en_movimiento_ = false;
		combatientes_[i]->casillas_movidas_ = 0;
		combatientes_[i]->casillas_movidas_x_ = 0;
		combatientes_[i]->casillas_movidas_y_ = 0;
		combatientes_[i]->velocidad_.x = 0;
		combatientes_[i]->velocidad_.y = 0;
	}	

	// colocacion en la arena
	pos_x_[0] = ARENA_MARGEN_X + 30.0f;
	pos_y_[0] = ARENA_MARGEN_Y + ZONA_DE_COMBATE_Y / 2.0f;
	pos_x_[1] = ARENA_MARGEN_X + ZONA_DE_COMBATE_X - 30.0f;
	pos_y_[1] = ARENA_MARGEN_Y + ZONA_DE_COMBATE_Y / 2.0f;

	for (int i = 0; i < 2; i++) {
		combatientes_[i]->setPosicion(Vector2D(pos_x_[i], pos_y_[i]));
		combatientes_[i]->setVelocidad(Vector2D(0, 0));

		vivo_[i] = true;
	}	
	
	for (int i = 0; i < 2; i++) {
		int tipo_de_ataque = ATAQUE_TIPO_GOLPE;
		if (tipo_de_ataque == ATAQUE_TIPO_GOLPE)   recarga_maxima_de_ataque_[i] = 0.5f;
		if (tipo_de_ataque == ATAQUE_TIPO_DISPARO) recarga_maxima_de_ataque_[i] = 1.0f;
		if (tipo_de_ataque == ATAQUE_EN_AREA)      recarga_maxima_de_ataque_[i] = 2.0f;
		recarga_de_ataque_[i] = 0;
	}

	ganador_ = -1;
	combate_terminado_ = false;

	colocarBarrerasAleatorias();
}

void Arena::actualizar(float dt) 
{
	if (combate_terminado_) return;
	actualizarBarreras(dt);
	actualizarMovimiento(dt);
	actualizarDisparo(dt);
	actualizarAtaque(dt);
	actualizarRecarga(dt);
	confirmarImpacto();
	confirmarFinCombate();
}

void Arena::recibirMovimiento(int jugador, int movimiento, bool tecla_pulsada) 
{
	if (movimiento == ARRIBA)    movimiento_arriba_[jugador] = tecla_pulsada;
	if (movimiento == ABAJO)     movimiento_abajo_[jugador] = tecla_pulsada;
	if (movimiento == IZQUIERDA) movimiento_izq_[jugador] = tecla_pulsada;
	if (movimiento == DERECHA)   movimiento_dch_[jugador] = tecla_pulsada;
}

bool Arena::recibirAtaque(int jugador, RenderizadorAudio* audio) 
{
	if (combate_terminado_) return false;
	if (!vivo_[jugador]) return false;
	if (recarga_de_ataque_[jugador] > 0) return false;

	int tipo = ATAQUE_TIPO_GOLPE; //combatientes[jugador]->getTipoAtaque();

	if (tipo == ATAQUE_TIPO_DISPARO) {
		direccion_disparo_x_[jugador] = ultima_direccion_x_[jugador];
		direccion_disparo_y_[jugador] = ultima_direccion_y_[jugador];
		disparo_x_[jugador] = pos_x_[jugador];
		disparo_y_[jugador] = pos_y_[jugador];
		disparo_disparado_[jugador] = true;

	}
	else if (tipo == ATAQUE_TIPO_GOLPE) {
		int rival = (jugador == 0) ? 1 : 0;

		// colocamos el palo delante de la gallina segun donde mire
		float offset = 18.0f;
		ataque_x_[jugador] = pos_x_[jugador] + ultima_direccion_x_[jugador] * offset;
		ataque_y_[jugador] = pos_y_[jugador] + ultima_direccion_y_[jugador] * offset;
		ataque_activo_[jugador] = true;
		ataque_visible_tiempo_[jugador] = 0;

		audio->playSonido("../assets/Audio/golpe.mp3");

		// comprobamos si el rival esta en rango de golpe
		float dx = pos_x_[jugador] - pos_x_[rival];
		float dy = pos_y_[jugador] - pos_y_[rival];
		float dist = sqrt(dx * dx + dy * dy);

		if (dist < 30.0f) {
			combatientes_[rival]->vida_ -= combatientes_[jugador]->ataque_;

			if (combatientes_[rival]->getEspecie() == GALLINA)
				audio->playSonido("../assets/Audio/damage_Gallina.mp3");
			else if (combatientes_[rival]->getEspecie() == CABRA)
				audio->playSonido("../assets/Audio/damage_Cabra.mp3");
			std::cout << "Jugador " << rival + 1 << " recibe golpe. Vida restante: "
				<< combatientes_[rival]->vida_ << std::endl;
		}
	}
	else if (tipo == ATAQUE_EN_AREA) {
		int rival = (jugador == 0) ? 1 : 0;
		float dx = pos_x_[jugador] - pos_x_[rival];
		float dy = pos_y_[jugador] - pos_y_[rival];
		float dist = sqrt(dx * dx + dy * dy);
		/*
		* float alcance = combatientes[jugador]->getAlcance();
		* if(dist<alcance){
		*   combatientes[rival]->recibirDano(combatientes[jugador]->getDano());
		* }
		*/
	}

	recarga_de_ataque_[jugador] = recarga_maxima_de_ataque_[jugador];
	return true;
}

bool Arena::combateTerminado() const { return combate_terminado_; }

int Arena::ganadorCombate()const { return ganador_; }

void Arena::actualizarMovimiento(float dt)
{
	for (int i = 0; i < 2; i++)
	{
		if (!vivo_[i]) continue;

		pos_antigua_x_[i] = pos_x_[i];
		pos_antigua_y_[i] = pos_y_[i];

		float velocidad = 100.0f;
		float dt_seg = dt / 1000.0f;  // convertimos a segundos

		if (movimiento_arriba_[i]) pos_y_[i] += velocidad * dt_seg;
		if (movimiento_abajo_[i])  pos_y_[i] -= velocidad * dt_seg;
		if (movimiento_izq_[i])    pos_x_[i] -= velocidad * dt_seg;
		if (movimiento_dch_[i])    pos_x_[i] += velocidad * dt_seg;

		if (movimiento_arriba_[i]) { ultima_direccion_x_[i] = 0; ultima_direccion_y_[i] = 1; }
		if (movimiento_abajo_[i]) { ultima_direccion_x_[i] = 0; ultima_direccion_y_[i] = -1; }
		if (movimiento_izq_[i]) { ultima_direccion_x_[i] = -1; ultima_direccion_y_[i] = 0; }
		if (movimiento_dch_[i]) { ultima_direccion_x_[i] = 1; ultima_direccion_y_[i] = 0; }

		if (colisionBarrera(pos_x_[i], pos_y_[i]))
		{
			pos_x_[i] = pos_antigua_x_[i];
			pos_y_[i] = pos_antigua_y_[i];
		}

	
		int rival = (i == 0) ? 1 : 0;
		float dx = pos_x_[i] - pos_x_[rival];
		float dy = pos_y_[i] - pos_y_[rival];
		float dist = sqrt(dx * dx + dy * dy);
		if (dist < 22.0f) 
		{
			pos_x_[i] = pos_antigua_x_[i];
			pos_y_[i] = pos_antigua_y_[i];
		}
		mantenerLimites(i);

		if (combatientes_[i] != nullptr) {
			combatientes_[i]->posicion_.x = pos_x_[i];
			combatientes_[i]->posicion_.y = pos_y_[i];
		}
	}
}

void Arena::actualizarDisparo(float dt) 
{
	float velocidad_proyectil = 200.0f;

	for (int i = 0; i < 2; i++) {
		if (!disparo_disparado_[i]) continue;

		disparo_x_[i] += direccion_disparo_x_[i] * velocidad_proyectil * dt;
		disparo_y_[i] += direccion_disparo_y_[i] * velocidad_proyectil * dt;

		// si sale de la pantalla lo desactivamos
		if (disparo_x_[i] < 0 || disparo_x_[i] > ANCHO_DE_LA_ARENA ||
			disparo_y_[i] < 0 || disparo_y_[i] > ALTO_DE_LA_ARENA) {
			disparo_disparado_[i] = false;
		}

		// si choca con una barrera lo desactivamos
		if (colisionBarrera(disparo_x_[i], disparo_y_[i])) {
			disparo_disparado_[i] = false;
		}
	}
}

void Arena::actualizarRecarga(float dt) 
{
	for (int i = 0; i < 2; i++) {
		if (recarga_de_ataque_[i] > 0)
			recarga_de_ataque_[i] -= dt/1000;
	}
}

void Arena::actualizarBarreras(float dt) 
{
	for (int i = 0; i < NUM_DE_BARRERAS; i++) {
		contador_ciclo_barrera_[i] += dt;
		if (contador_ciclo_barrera_[i] >= ciclo_maximo_barrera_[i]) 
		{
			contador_ciclo_barrera_[i] = 0.0f;
			barrera_visible_[i] = !barrera_visible_[i];
			ciclo_maximo_barrera_[i] = 10000.0f + (rand() % 10);

			if (barrera_visible_[i]) 
			{
				for (int j = 0; j < 2; j++) 
				{
					if (!vivo_[j]) continue;
					float dx = pos_x_[j] - barrera_x_[i];
					float dy = pos_y_[j] - barrera_y_[i];
					if (dx > -10 && dx < 10 && dy > -12 && dy < 12) 
					{
						// empujamos al jugador fuera de la barrera
						// lo movemos hacia el lado mas cercano
						if (abs(dx) > abs(dy))
							pos_x_[j] += (dx > 0) ? 12.0f : -12.0f;
						else
							pos_y_[j] += (dy > 0) ? 14.0f : -14.0f;

						// sincronizamos con el animal
						if (combatientes_[j] != nullptr) 
						{
							combatientes_[j]->posicion_.x = pos_x_[j];
							combatientes_[j]->posicion_.y = pos_y_[j];
						}
					}
				}
			}
		}
	}
}


void Arena::confirmarImpacto() 
{
	for (int i = 0; i < 2; i++) {
		if (!disparo_disparado_[i]) continue;

		int rival = (i == 0) ? 1 : 0;
		if (!vivo_[rival]) continue;

		float dx = disparo_x_[i] - pos_x_[rival];
		float dy = disparo_y_[i] - pos_y_[rival];
		float dist = sqrt(dx * dx + dy * dy);

		if (dist < 15.0f) { // radio de impacto
			//combatientes[rival]->recibirDano(combatientes[i]->getDano());
			disparo_disparado_[i] = false;
		}
	}
}

void Arena::confirmarFinCombate() 
{
	for (int i = 0; i < 2; i++) {
		if (combatientes_[i] != nullptr && combatientes_[i]->vida_ <= 0) {
			vivo_[i] = false;
			combate_terminado_ = true;
			ganador_ = (i == 0) ? 1 : 0;
			std::cout << "Jugador " << ganador_ + 1 << " gana el combate!" << std::endl;
		}
	}
}

void Arena::mantenerLimites(int jugador)
{
	float radio = 11.0f;
	float antes_x = pos_x_[jugador];
	float antes_y = pos_y_[jugador];

	if (pos_x_[jugador] < ARENA_MARGEN_X + radio) pos_x_[jugador] = ARENA_MARGEN_X + radio;
	if (pos_x_[jugador] > ARENA_MARGEN_X + ZONA_DE_COMBATE_X - radio) pos_x_[jugador] = ARENA_MARGEN_X + ZONA_DE_COMBATE_X - radio;
	if (pos_y_[jugador] < ARENA_MARGEN_Y + radio) pos_y_[jugador] = ARENA_MARGEN_Y + radio;
	if (pos_y_[jugador] > ARENA_MARGEN_Y + ZONA_DE_COMBATE_Y - radio) pos_y_[jugador] = ARENA_MARGEN_Y + ZONA_DE_COMBATE_Y - radio;

	if (antes_x != pos_x_[jugador] || antes_y != pos_y_[jugador])
		std::cout << "LIMITE ACTIVADO J" << jugador << ": de (" << antes_x << "," << antes_y << ") a (" << pos_x_[jugador] << "," << pos_y_[jugador] << ")" << std::endl;
}

bool Arena::colisionBarrera(float x, float y) 
{
	for (int i = 0; i < NUM_DE_BARRERAS; i++) 
	{
		if (!barrera_visible_[i]) continue;
		float dx = x - barrera_x_[i];
		float dy = y - barrera_y_[i];
		if (dx > -10 && dx < 10 && dy > -12 && dy < 12)
			return true;
	}
	return false;
}

void Arena::colocarBarrerasAleatorias() 
{
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
		barrera_x_[i] = origen_x + (rand() % (int)(zona_x * 0.7f));
		barrera_y_[i] = origen_y + (rand() % (int)(zona_y * 0.7f));

	}
}

void Arena::actualizarAtaque(float dt)
{
	for (int i = 0; i < 2; i++) {
		if (!ataque_activo_[i]) continue;

		ataque_visible_tiempo_[i] += dt/1000;

		// el palo desaparece despues de DURACION_ATAQUE segundos
		if (ataque_visible_tiempo_[i] >= DURACION_ATAQUE)
		{
			ataque_activo_[i] = false;
			ataque_visible_tiempo_[i] = 0;
		}
	}
}
	
int Arena::obtenerPerdedor() const 
{
	return (ganador_ == 0) ? 1 : 0;
}

