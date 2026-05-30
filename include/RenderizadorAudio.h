#pragma once
#include "ETSIDI.h"
#include "animal.h"

class Menu;
class Tablero;
class Arena;
class Creditos;
class Controles;

class RenderizadorAudio
{
    //guardamos la ruta de la musica que esta sonando ahora mismo
    mutable const char* musica_actual_ = nullptr;
    void cambiarMusica(const char* rutaAudio) const;
public:
    
    void sonar(const Menu* menu) const;
    void sonar(const Tablero* tablero) const;
    void sonar(const Arena* arena) const;
    void sonar(const Creditos* creditos) const;
    void sonar(const Controles* controles) const;

 
    void sonarTransicion() const;
    void sonarPickeo(const Animal* animal) const;
    void sonarAtaque(int jugador, const Animal* atacante) const;
    void sonarHuevo(const Animal* atacante) const;
    void sonarAtacado(const Animal* atacado) const;
    void sonidoMenu() const;
    void eleccionMenu() const;
    
    void playMusica(const char* rutaAudio, bool repitir = true) const;
    void stopMusica() const;
    void playSonido(const char* rutaAudio) const;
};