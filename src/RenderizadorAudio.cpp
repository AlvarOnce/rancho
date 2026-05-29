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
        ETSIDI::play("../assets/Audio/sonidoPickeoGallina.mp3");
    else if (animal->getEspecie() == CABRA)
        ETSIDI::play("../assets/Audio/sonidoPickeoCabra.mp3");
}

void RenderizadorAudio::sonarAtaque(const Animal* animal) const
{
    ETSIDI::play("../assets/Audio/golpe.mp3");
}

void RenderizadorAudio::sonarDanoGallina() const
{
    ETSIDI::play("../assets/Audio/damage_Gallina.mp3");
}

void RenderizadorAudio::sonarDanoCabra() const
{
    ETSIDI::play("../assets/Audio/damage_Cabra.mp3");
}
void RenderizadorAudio::sonarTransicion() const
{
    ETSIDI::play("../assets/Audio/transicion.mp3");
}