#pragma once

class Transicion
{
public:

    enum Estado { ESPERANDO, CERRANDO, CERRADO, ABRIENDO };

private:

    bool activo = false;
    float tamano = 600;
    Estado estado = ESPERANDO;

public:

    void empieza() { activo = true; }
    void termina() { activo = false; }
    void actualizar(float dt);

    bool getActivo() const { return activo; }
    float getTamano() const { return tamano; }
    Estado getEstado() const { return estado; }
};