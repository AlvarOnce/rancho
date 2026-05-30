#include "RenderizadorAudio.h"
#include <cstring>

void RenderizadorAudio::cambiarMusica(const char* rutaAudio) const
{
    if (musica_actual_ != nullptr && strcmp(musica_actual_, rutaAudio) == 0)
        return;

    // si es diferente la cambiamos
    musica_actual_ = rutaAudio;
    ETSIDI::playMusica(rutaAudio, true);
}

void RenderizadorAudio::sonar(const Menu* menu) const
{
    if (musica_actual_ == nullptr)
        cambiarMusica("../assets/Audio/menu.mp3");
}

void RenderizadorAudio::sonar(const Tablero* tablero) const
{
    ETSIDI::playMusica("../assets/Audio/musica_tablero.mp3", true);
}

void RenderizadorAudio::sonar(const Arena* arena) const
{
    ETSIDI::playMusica("../assets/Audio/musica_combate.mp3", true);
}

void RenderizadorAudio::sonar(const Creditos* creditos) const
{
    //sigue sonando la musica
}

void RenderizadorAudio::sonar(const Controles* controles) const
{
    //igual que creditos
}

void RenderizadorAudio::playMusica(const char* rutaAudio, bool repitir) const
{
    ETSIDI::playMusica(rutaAudio, repitir);
}

void RenderizadorAudio::stopMusica() const
{
    musica_actual_ = nullptr;
    ETSIDI::stopMusica();
}

void RenderizadorAudio::playSonido(const char* rutaAudio) const
{
    ETSIDI::play(rutaAudio);
}

void RenderizadorAudio::sonarPickeo(const Animal* animal) const
{
    if (animal->getEspecie() == GALLINA)
        ETSIDI::play("../assets/Audio/gallina.mp3");
    else if (animal->getEspecie() == CABRA)
        ETSIDI::play("../assets/Audio/cabra.mp3");
    else if (animal->getEspecie() == CERDO)
        ETSIDI::play("../assets/Audio/cerdo.mp3");
    else if (animal->getEspecie() == LLAMA)
        ETSIDI::play("../assets/Audio/llama.mp3");
    else if (animal->getEspecie() == OVEJA)
        ETSIDI::play("../assets/Audio/oveja.mp3");
    else if (animal->getEspecie() == GRANJERO)
        ETSIDI::play("../assets/Audio/granjero.mp3");
}

void RenderizadorAudio::sonarAtaque(int jugador, const Animal* atacante) const
{
    if (atacante->getEspecie() == GALLINA)
        ETSIDI::play("../assets/Audio/lanzarhuevo.mp3");
    else if (atacante->getEspecie() == CABRA)
        ETSIDI::play("../assets/Audio/embestida.mp3");
    else if (atacante->getEspecie() == GRANJERO)
        ETSIDI::play("../assets/Audio/granjeroataque.mp3");
    else if (atacante->getEspecie() == LLAMA)
        ETSIDI::play("../assets/Audio/escupidaLlama.mp3");
    else if (atacante->getEspecie() == CERDO)
        ETSIDI::play("../assets/Audio/ataquecerdo.mp3");
    else if (atacante->getEspecie() == OVEJA)
        ETSIDI::play("../assets/Audio/ataqueoveja.mp3");
}

void RenderizadorAudio::sonarHuevo(const Animal* atacante) const
{
    if (atacante->getEspecie() == GALLINA)
        ETSIDI::play("../assets/Audio/huevoroto.mp3");
    if (atacante->getEspecie() == LLAMA)
        ETSIDI::play("../assets/Audio/impactobabas.mp3");
}

void RenderizadorAudio::sonarAtacado(const Animal* atacado) const
{
    if (atacado->getEspecie() == GALLINA)
        ETSIDI::play("../assets/Audio/damage_gallina.mp3");
    if (atacado->getEspecie() == CABRA)
        ETSIDI::play("../assets/Audio/damage_cabra.mp3");
    if (atacado->getEspecie() == GRANJERO)
        ETSIDI::play("../assets/Audio/damage_hombre.mp3");
    if (atacado->getEspecie() == CERDO)
        ETSIDI::play("../assets/Audio/damage_cerdo.mp3");
    if (atacado->getEspecie() == OVEJA)
        ETSIDI::play("../assets/Audio/damage_oveja.mp3");
    if (atacado->getEspecie() == LLAMA)
        ETSIDI::play("../assets/Audio/damage_llama.mp3");
}

void RenderizadorAudio::sonidoMenu() const
{
    ETSIDI::play("../assets/Audio/mover_menu.mp3");
}

void RenderizadorAudio::eleccionMenu() const
{
    ETSIDI::play("../assets/Audio/seleccion_menu.mp3");
}


void RenderizadorAudio::sonarTransicion() const
{
    ETSIDI::play("../assets/Audio/transicion.mp3");
}