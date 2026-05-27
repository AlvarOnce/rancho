#include "menu.h"

#include <string>
#include <iostream>
#include "math.h"

void Menu::actualizar(float dt) 
{
	angulo += 0.05; 
	if (angulo > 360) angulo = 0; // LETRAS

	for (int i = 0; i < 6; i++) // LETRAS 
	{
		titulo[i].altura = 0.73333 * alto + 0.04 * alto * sin(angulo + i / 2.0); // Desplazamientos relativos a ancho y alto
		titulo[i].horiz = 0.05625 * ancho + 0.1125 * ancho * i;
		if (i == 0)  titulo[i].horiz -= 0.00625 * ancho; // Ajuste a la izquierda por ser la 'R' más grande
	}

	if (tractor.posx > -200) tractor.posx -= 0.8; // TRACTOR
	tractor.animar(dt);

	for (auto& n : nube) // NUBES 
		if (n.posx <480) n.posx += 0.2;
		else n.posx = 0;

	if (paloma.posx < 0.15 * ancho + titulo[0].horiz) // PALOMA (Es posible hacer paloma.actualizar(dt) y llamar aquí)
	{
		paloma.posx++;
		paloma.animar(dt);
	}		
	else
	{
		paloma.frameActualX_ = 0;  // Hacer SetState para una clase general OBJETO para no redefinir siempre
		paloma.frameActualY_ = 0;
		paloma.posx = 0.15 * ancho + titulo[0].horiz;
		paloma.posy = 0.14444 * alto + titulo[0].altura; 
	}

	selector.posy = 139 - (selector.opcionActual * 25.0f); // SELECTOR	
	selector.tamano_actual = selector.tamano_base + 1.0f * sin(angulo * 3.0f); // latido del selector
}



void Menu::moverSelector(int direccion) 
{
	int nueva_opcion = selector.opcionActual + direccion;
	
	if (nueva_opcion >= 0 && nueva_opcion <= 3) {
		selector.opcionActual = static_cast<Selector::Opcion>(nueva_opcion);
	}
}

void Paloma::animar(float dt) 
{
	timer = timer + dt;
	if (timer > msStep)
	{
		if (frameActualX_ < nFrames - 1) frameActualX_++;
		else frameActualX_ = 0;
		timer = timer - msStep;
	}
}

void Tractor::animar(float dt) 
{
	timer = timer + dt;
	if (timer > msStep)
	{
		if (frameActualX_ < nFrames - 1) frameActualX_++;
		else frameActualX_ = 0;
		timer = timer - msStep;
	}
}






