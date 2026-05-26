#pragma once

class Ataque {

protected:
    int dano_;
    float alcance_;
    const char* tipo_;

    // Visualización
    const char* sprite_;      
    float tamanio_;           
    float duracion_;          
    float r_, g_, b_;         

public:
    virtual ~Ataque() {}

    int getDano() const { return dano_; }
    float getAlcance() const { return alcance_; }
    const char* getTipo() const { return tipo_; }

    // Visualización
    const char* getSprite() const { return sprite_; }
    float getTamanio() const { return tamanio_; }
    float getDuracion() const { return duracion_; }
    void getColor(float& r, float& g, float& b) const {
        r = r_; g = g_; b = b_;
    }
};
