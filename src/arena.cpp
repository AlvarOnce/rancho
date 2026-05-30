#include "arena.h"
#include "interaccion.h"
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>

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
		recarga_de_ataque_[i] = 0;
		ultima_direccion_x_[i] = 0;
		ultima_direccion_y_[i] = 0;
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

void Arena::inicioCombate()
{
	// desactivamos toda la logica del tablero para estos animales
	for (int i = 0; i < 2; i++) {
		combatientes_[i]->intro_tablero_ = false;
		combatientes_[i]->en_movimiento_ = false;
		combatientes_[i]->casillas_movidas_ = 0;
		combatientes_[i]->casillas_movidas_x_ = 0;
		combatientes_[i]->casillas_movidas_y_ = 0;

		combatientes_[i]->setVelocidad(Vector2D(0, 0)); // por resolver
	}	

	// colocacion en la arena
	pos_x_[0] = ARENA_MARGEN_X + 30.0f;
	pos_y_[0] = ARENA_MARGEN_Y + ZONA_DE_COMBATE_Y / 2.0f;
	pos_x_[1] = ARENA_MARGEN_X + ZONA_DE_COMBATE_X - 30.0f;
	pos_y_[1] = ARENA_MARGEN_Y + ZONA_DE_COMBATE_Y / 2.0f;

	ultima_direccion_x_[0] = 1.0f; ultima_direccion_y_[0] = 0.0f;
	ultima_direccion_x_[1] = -1.0f; ultima_direccion_y_[1] = 0.0f;

	for (int i = 0; i < 2; i++) {
		combatientes_[i]->setPosicion(Vector2D(pos_x_[i], pos_y_[i])); // develop: Vector2D
		combatientes_[i]->capaz_ = -4.5f;
		vivo_[i] = true;
		recarga_de_ataque_[i] = 0;
		if (combatientes_[i]->getAtaque())
			combatientes_[i]->getAtaque()->desactivar();
	}

	ganador_ = -1;
	combate_terminado_ = false;
	colocarBarrerasAleatorias();
}

void Arena::actualizar(float dt)
{

	if (intro_arena)
	{
		inicioCombate();
		intro_arena = false;
	}

	if (combate_terminado_) {
		intro_arena = true;
		return;
	}

	actualizarBarreras(dt);
	actualizarMovimiento(dt);
	actualizarAtaques(dt);
	actualizarRecarga(dt);
	confirmarImpacto();
	confirmarFinCombate();

	combatientes_[0]->actualizarEnBatalla(dt);
	combatientes_[1]->actualizarEnBatalla(dt);
}

void Arena::recibirMovimiento(int jugador, int movimiento, bool tecla_pulsada)
{
	if (movimiento == ARRIBA)    movimiento_arriba_[jugador] = tecla_pulsada;
	if (movimiento == ABAJO)     movimiento_abajo_[jugador] = tecla_pulsada;
	if (movimiento == IZQUIERDA) movimiento_izq_[jugador] = tecla_pulsada;
	if (movimiento == DERECHA)   movimiento_dch_[jugador] = tecla_pulsada;
}
bool Arena::recibirAtaque(int jugador)
{
	if (combate_terminado_) return false;
	if (!vivo_[jugador]) return false;
	if (recarga_de_ataque_[jugador] > 0) return false;

	Ataque* ataque = combatientes_[jugador]->getAtaque();
	if (!ataque) return false;

	float offsetX = pos_x_[jugador] + ultima_direccion_x_[jugador] * 18.0f;
	float offsetY = pos_y_[jugador] + ultima_direccion_y_[jugador] * 18.0f;
	ataque->activar(offsetX, offsetY,ultima_direccion_x_[jugador],ultima_direccion_y_[jugador]);
	//audio->sonarAtaque(combatientes_[jugador]);
	recarga_de_ataque_[jugador] = ataque->getRecarga();
	//if (combatientes_[rival]->getEspecie() == GALLINA)
			//audio->sonarDanoGallina();
		//else if (combatientes_[rival]->getEspecie() == CABRA)
			//audio->sonarDanoCabra();
	return true;
}

void Arena::actualizarMovimiento(float dt)
{
	for (int i = 0; i < 2; i++)
	{
		if (!vivo_[i]) continue;

		pos_antigua_x_[i] = pos_x_[i];
		pos_antigua_y_[i] = pos_y_[i];

		const float velocidad = 100.0f;
		const float dt_seg = dt / 1000.0f;  // convertimos a segundos

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
			combatientes_[i]->setPosicion(Vector2D(pos_x_[i], pos_y_[i]));
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
void Arena::actualizarAtaques(float dt)
{
	for (int i = 0; i < 2; i++) {
		Ataque* ataque = combatientes_[i] ? combatientes_[i]->getAtaque() : nullptr;
		if (!ataque || !ataque->isActivo()) continue;

		ataque->mover(dt);
		if (!ataque->isActivo()) continue;

		if (ataque->getX() < LIMITE_IZQ_ARENA || ataque->getX() > LIMITE_DCH_ARENA ||
			ataque->getY() < LIMITE_ARRIBA_ARENA || ataque->getY() > LIMITE_ABAJO_ARENA)
			ataque->desactivar();

		if (colisionBarrera(ataque->getX(), ataque->getY()))
			ataque->desactivar();
	}
}

void Arena::actualizarBarreras(float dt) 
{
	for (int i = 0; i < NUM_DE_BARRERAS; i++) {
		contador_ciclo_barrera_[i] += dt;
		if (contador_ciclo_barrera_[i] >= ciclo_maximo_barrera_[i]) 

	
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

void Arena::confirmarImpacto() 
{
	for (int i = 0; i < 2; i++) {
		Ataque* ataque = combatientes_[i] ? combatientes_[i]->getAtaque() : nullptr;
		if (!ataque || !ataque->isActivo()) continue;

		int rival = (i == 0) ? 1 : 0;
		if (!vivo_[rival]) continue;

		if (Interaccion::procesarImpacto(ataque, combatientes_[rival])) {
			std::cout << "Jugador " << rival + 1 << " recibe "
				<< combatientes_[i]->getTipoAtaque()
				<< " de " << ataque->getDano()
				<< " dano. Vida: " << combatientes_[rival]->vida_ << std::endl;
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

	if (pos_x_[jugador] < ARENA_MARGEN_X + radio) pos_x_[jugador] = ARENA_MARGEN_X + radio;
	if (pos_x_[jugador] > ARENA_MARGEN_X + ZONA_DE_COMBATE_X - radio) pos_x_[jugador] = ARENA_MARGEN_X + ZONA_DE_COMBATE_X - radio;
	if (pos_y_[jugador] < ARENA_MARGEN_Y + radio) pos_y_[jugador] = ARENA_MARGEN_Y + radio;
	if (pos_y_[jugador] > ARENA_MARGEN_Y + ZONA_DE_COMBATE_Y - radio) pos_y_[jugador] = ARENA_MARGEN_Y + ZONA_DE_COMBATE_Y - radio;
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
int Arena::obtenerPerdedor() const
{
	// Si el combate no ha terminado, no hay perdedor todavía
	if (!combate_terminado_) return -1;

	// Si el ganador es el jugador 0 (Luz), el perdedor es el 1 (Oscuridad)
	// Si el ganador es el jugador 1, el perdedor es el 0
	if (ganador_ == 0) return 1;
	if (ganador_ == 1) return 0;

	return -1; // Por si acaso hubo un empate o error
}
