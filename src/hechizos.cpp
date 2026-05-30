#include "tablero.h"

void Tablero::procesarTeclaHechizo(int tecla) {
    Jugador* jugadorActivo = getJugadorActivo();

    // solo se pueden lanzar hechizos si tienes a alguien agarrado (de nada, chile)
    if (!jugadorActivo->tienePiezaAgarrada()) return;

    Animal* pieza = jugadorActivo->getPiezaSeleccionada();

    // solo el granjero puede lanzar hechizos
    if (pieza->getEspecie() != GRANJERO) return;

    // si pulsas la misma tecla con la que activaste el hechizo, se cancela
    if (teclaHechizoActivo_ == tecla) {
        std::cout << "Hechizo cancelado. Mueve el Granjero o elige otro hechizo.\n";
        estadoHechizo_ = INACTIVO;
        teclaHechizoActivo_ = -1;
        primerObjetivoHechizo_ = nullptr;
        return;
    }

    int bando = jugadorActivo->getEquipo(); // 0 luz, 1 oscuridad
    int tipoHechizo = 0;

    // teclas: J1 usa 1-5, J2 usa 6-0
    if (bando == 0) {
        if (tecla >= 1 && tecla <= 5) tipoHechizo = tecla;
        else return;
    }
    else {
        if (tecla >= 6 && tecla <= 9) tipoHechizo = tecla - 5;
        else if (tecla == 0) tipoHechizo = 5;
        else return;
    }

    // validación de si el bando ya ha gastado el hechizo elegido
    if (!hechizoDisponible_[bando][tipoHechizo]) {
        std::cout << "[!] El Granjero ya ha usado este hechizo.\n";
        estadoHechizo_ = INACTIVO;
        teclaHechizoActivo_ = -1;
        return;
    }

    teclaHechizoActivo_ = tecla;
    std::cout << "\n--- Jugador " << (bando + 1) << " usando hechizo ---\n";

    // asignar el hechizo y guiar al usuario, 
    // esto de momento en consola, habría que usar ETSIDILIB para escribirlo en pantalla en algún sitio libre
    switch (tipoHechizo) {
    case 1: // TELETRANSPORTE
        estadoHechizo_ = TELETRANSPORTE_SELECCIONAR_ALIADO;
        std::cout << "HECHIZO: Teletransporte.\n";
        std::cout << "> Selecciona un animal de tu equipo.\n";
        std::cout << "> Pulsa " << tecla << " para cancelar.\n";
        break;
    case 2: // CURAR
        estadoHechizo_ = CURAR_SELECCIONAR_ALIADO;
        std::cout << "HECHIZO: Curar.\n";
        std::cout << "> Selecciona un animal de tu equipo para sanarlo al maximo.\n";
        std::cout << "> Pulsa " << tecla << " para cancelar.\n";
        break;
    case 3: // INTERCAMBIO
        estadoHechizo_ = INTERCAMBIO_SELECCIONAR_ALIADO;
        std::cout << "HECHIZO: Intercambio.\n";
        std::cout << "> Selecciona un animal de tu equipo.\n";
        std::cout << "> Pulsa " << tecla << " para cancelar.\n";
        break;
    case 4: // ATRAPAR
        estadoHechizo_ = ATRAPAR_SELECCIONAR_ENEMIGO;
        std::cout << "HECHIZO: Atrapar.\n";
        std::cout << "> Selecciona un animal rival para inmovilizarlo.\n";
        std::cout << "> Pulsa " << tecla << " para cancelar.\n";
        break;
    default:
        std::cout << "Hechizo " << tipoHechizo << " no existe.\n";
        estadoHechizo_ = INACTIVO;
        teclaHechizoActivo_ = -1;
        break;
    }
}

void Tablero::ejecutarPasoHechizo(Animal* casilla, int fila, int col) {
    Jugador* jugadorActivo = getJugadorActivo();
    int bando = jugadorActivo->getEquipo();

    switch (estadoHechizo_) {
    case TELETRANSPORTE_SELECCIONAR_ALIADO:
        if (casilla && casilla->getEquipo() == bando && casilla->getEspecie() != GRANJERO) {
            primerObjetivoHechizo_ = casilla;
            estadoHechizo_ = TELETRANSPORTE_SELECCIONAR_DESTINO;
            std::cout << "> Animal Aliado seleccionado.\n> Ahora mueve el cursor a una cailla vacia y seleccionala como destino.\n";
        }
        else {
            std::cout << "[!] Debes seleccionar un animal de tu equipo (que no sea el Granjero).\n";
        }
        break;

    case TELETRANSPORTE_SELECCIONAR_DESTINO:
        if (casilla == nullptr) { // Casilla vacía
            casillas_[fila][col] = primerObjetivoHechizo_;
            casillas_[primerObjetivoHechizo_->casillaInicial_.fila][primerObjetivoHechizo_->casillaInicial_.columna] = nullptr;

            primerObjetivoHechizo_->casillaInicial_ = { fila, col };
            float nuevaPosX = 141.0f + 11.0f + (22.0f * col);
            float nuevaPosY = 36.0f + 11.0f + (22.0f * (8 - fila));
            primerObjetivoHechizo_->setPosicion(Vector2D(nuevaPosX, nuevaPosY));

            std::cout << ">> Teletransporte completado con exito!\n";
            hechizoDisponible_[bando][1] = false; // se consume teletransporte
            finalizarHechizo();
        }
        else {
            std::cout << "[!] La parcela de destino debe estar vacia.\n";
        }
        break;

    case CURAR_SELECCIONAR_ALIADO:
        if (casilla && casilla->getEquipo() == bando) {
            casilla->vida_ = 10; // FALTA PONER LA VIDA MAXIMA DE CADA ANIMAL, habrá que hacer un getVidaMaxima() o algo así
            std::cout << ">> Animal curado al maximo de vida!\n";
            hechizoDisponible_[bando][2] = false; // se consume curar
            finalizarHechizo();
        }
        else {
            std::cout << "[!] Debes seleccionar un animal de tu equipo.\n";
        }
        break;

    case INTERCAMBIO_SELECCIONAR_ALIADO:
        if (casilla && casilla->getEquipo() == bando && casilla->getEspecie() != GRANJERO) {
            primerObjetivoHechizo_ = casilla;
            estadoHechizo_ = INTERCAMBIO_SELECCIONAR_ENEMIGO;
            std::cout << "> Animal Aliado seleccionado.\n> Ahora mueve el cursor y selecciona un animal RIVAL.\n";
        }
        else {
            std::cout << "[!] Selecciona un animal de tu equipo (que no sea el Granjero).\n";
        }
        break;

    case INTERCAMBIO_SELECCIONAR_ENEMIGO:
        if (casilla && casilla->getEquipo() != bando) {
            Casilla pos1 = primerObjetivoHechizo_->casillaInicial_;
            Casilla pos2 = casilla->casillaInicial_;

            // intercambio en la matriz
            casillas_[pos2.fila][pos2.columna] = primerObjetivoHechizo_;
            casillas_[pos1.fila][pos1.columna] = casilla;

            // intercambio de atributos lógicos
            primerObjetivoHechizo_->casillaInicial_ = pos2;
            casilla->casillaInicial_ = pos1;

            // actualizar física aliado
            float nuevaPosX1 = 141.0f + 11.0f + (22.0f * pos2.columna);
            float nuevaPosY1 = 36.0f + 11.0f + (22.0f * (8 - pos2.fila));
            primerObjetivoHechizo_->setPosicion(Vector2D(nuevaPosX1, nuevaPosY1));

            // actualizar física rival
            float nuevaPosX2 = 141.0f + 11.0f + (22.0f * pos1.columna);
            float nuevaPosY2 = 36.0f + 11.0f + (22.0f * (8 - pos1.fila));
            casilla->setPosicion(Vector2D(nuevaPosX2, nuevaPosY2));

            std::cout << ">> Intercambio completado!\n";
            hechizoDisponible_[bando][3] = false; // se consume intercambio
            finalizarHechizo();
        }
        else {
            std::cout << "[!] Debes seleccionar un animal rival.\n";
        }
        break;

    case ATRAPAR_SELECCIONAR_ENEMIGO:
        if (casilla && casilla->getEquipo() != bando) {
            casilla->atrapado_ = true;
            casilla->ciclos_atrapado_ = 3;
            std::cout << ">> Animal rival atrapado e inmovilizado por 3 turnos!\n";
            hechizoDisponible_[bando][4] = false; // se consume atrapar
            finalizarHechizo();
        }
        else {
            std::cout << "[!] Debes seleccionar un animal rival.\n";
        }
        break;

    default:
        break;
    }
}

void Tablero::finalizarHechizo() {
    Jugador* jugadorActivo = getJugadorActivo();
    Animal* granjero = jugadorActivo->getPiezaSeleccionada();

    // devolver el granjero a su casilla original ya que castear el hechizo consume su acción
    float origX = 141.0f + 11.0f + (22.0f * granjero->casillaInicial_.columna);
    float origY = 36.0f + 11.0f + (22.0f * (8 - granjero->casillaInicial_.fila));
    granjero->setPosicion(Vector2D(origX, origY));
    casillas_[granjero->casillaInicial_.fila][granjero->casillaInicial_.columna] = granjero;

    jugadorActivo->soltarPieza();

    // reset de la  máquina de estados
    estadoHechizo_ = INACTIVO;
    teclaHechizoActivo_ = -1;
    primerObjetivoHechizo_ = nullptr;

    avanzarTurnosAtrapados(); // antes de pasar el turno

    // cambio de turno
    turno_actual_ = (turno_actual_ == 0) ? 1 : 0;
    letreroTurnos_.setState(0, turno_actual_);    
}

void Tablero::avanzarTurnosAtrapados() {
    for (int i = 0; i < Constantes::FILAS_TABLERO; i++) {
        for (int j = 0; j < Constantes::COLUMNAS_TABLERO; j++) {
            Animal* anim = casillas_[i][j];
            // disminuye en 1 si es el turno del jugador al que pertenece el animal atrapado
            if (anim != nullptr && anim->atrapado_ && anim->equipo_ == turno_actual_) {
                anim->ciclos_atrapado_--;
                if (anim->ciclos_atrapado_ <= 0) {
                    anim->atrapado_ = false;
                    std::cout << "\n>> Un animal atrapado del jugador " << (turno_actual_ + 1) << " se ha liberado!\n";
                }
            }
        }
    }
}