#include "RenderizadorAudio.h"

void RenderizadorAudio::playMusica(const char* rutaAudio, bool repitir) const
{
    ETSIDI::playMusica(rutaAudio, repitir);
}

void RenderizadorAudio::stopMusica() const
{
    ETSIDI::stopMusica();
}

void RenderizadorAudio::playSonido(const char* rutaAudio) const
{
    ETSIDI::play(rutaAudio);
}