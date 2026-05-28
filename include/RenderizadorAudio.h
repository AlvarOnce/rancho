#pragma once

#include "ETSIDI.h"

class RenderizadorAudio
{
public:

 
    void playMusica(const char* rutaAudio, bool repite = true) const;
    void stopMusica() const;
    void playSonido(const char* rutaAudio) const;
};