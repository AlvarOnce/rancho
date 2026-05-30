#include "renderizador.h"
#include <iostream>
#include <string>
#include <cmath>

// LIMPIAR PANTALLA
void Renderizador::limpiarPantalla()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// ANIMALES
void Renderizador::dibujar(const Animal* animal) const
{
    if (!animal || !animal->getVivo()) return;

	std::string ruta_sprite;
    switch (animal->getEspecie()) {

        case CABRA:
            if (animal->getEquipo() == 0) ruta_sprite = "../assets/Sprites/cabra/cabraSpritesheet.png";
            else ruta_sprite = "../assets/Sprites/cabra/cabraSpritesheet1.png";
            break;

        case GALLINA:
            if (animal->getEquipo() == 0) ruta_sprite = "../assets/Sprites/gallina/gallinaSpritesheet.png";
            else ruta_sprite = "../assets/Sprites/gallina/gallinaSpritesheet1.png";
            break;

        case GRANJERO:
            if(animal->getEquipo() == 0) ruta_sprite = "../assets/Sprites/granjero/granjeroSpritesheet.png";
            else ruta_sprite = "../assets/Sprites/granjero/granjeroSpritesheet1.png";
            break;

        case OVEJA:
            if (animal->getEquipo() == 0) ruta_sprite = "../assets/Sprites/oveja/ovejaSpritesheet.png";
            else ruta_sprite = "../assets/Sprites/oveja/ovejaSpritesheet1.png";
            break;

        case CERDO:
            if (animal->getEquipo() == 0) ruta_sprite = "../assets/Sprites/cerdo/cerdoSpritesheet.png";
            else ruta_sprite = "../assets/Sprites/cerdo/cerdoSpritesheet1.png";
            break;

        case LLAMA:
            if (animal->getEquipo() == 0) ruta_sprite = "../assets/Sprites/llama/llamaSpritesheet.png";
            else ruta_sprite = "../assets/Sprites/llama/llamaSpritesheet1.png";
            break;

        default:
            ruta_sprite = "../assets/Sprites/gallina/gallinaSpritesheet.png";
            break;
    }

    dibujarSprite(
       ruta_sprite.c_str(),
        256, 128,
        animal->getPosX(), animal->getPosY(),
        animal->getCapaz(),
        4, 8,
        animal->getFrameActualX(), animal->getFrameActualY(),
        true
	);
}

// TABLERO
void Renderizador::dibujar(const Tablero* tablero) const
{
	// DIBUJAR FONDO Y LETRERO DE TURNOS
    dibujarSprite("../assets/Sprites/tablero/tableroFondo.png", 512, 512, 480 / 2, 270 / 2, -1);
    dibujarSprite("../assets/Sprites/tablero/tablero.png", 256, 256, 480 / 2, 270 / 2, -2);
    dibujarSprite("../assets/Sprites/tablero/turnos.png", 256, 128,
        tablero->getLetreroPosX(), 270 / 2, -5, 4, 8,
        tablero->getLetreroFrameX(), tablero->getLetreroFrameY());

    // ILUMINAR CASILLAS POSIBLES
	// referencias constantes para no modificar los objetos originales
    const Cursor& cursorActivo = tablero->getCursorActivo();
    const Jugador* jugadorActivo = tablero->getJugadorActivo();
    
    int filaCursor = cursorActivo.getFila();
    int colCursor = cursorActivo.getColumna();
    
    const Animal* animalEnCursor = tablero->getAnimalEnCasilla(filaCursor, colCursor);
    bool tienePiezaAgarrada = jugadorActivo->tienePiezaAgarrada();
    if (tablero->getEstadoHechizo() == INACTIVO) // para que no aparezcan mientras se usa un hechizo
    {
        if (animalEnCursor != nullptr || tienePiezaAgarrada) {

        int maxMovimientos = 0;
        int equipoFicha = -1;
        float deltaTamanoTarjeta = 4 * sin(2 * tablero->angulo);

        if (animalEnCursor != nullptr) {
            maxMovimientos = animalEnCursor->getMaxCasillasMovidas();
            equipoFicha = animalEnCursor->getEquipo();

            // dibujar tarjeta de animal en cursor
         
            dibujarSprite("../assets/Sprites/tarjetas/tarjetas.png", 256 + deltaTamanoTarjeta, 512 + deltaTamanoTarjeta/2.0f, 69 + (342* equipoFicha), 32, -5.5, 8, 2, equipoFicha, animalEnCursor->getEspecie());
        }

        if (tienePiezaAgarrada) {
            const Animal* piezaSeleccionada = jugadorActivo->getPiezaSeleccionada();
            maxMovimientos = piezaSeleccionada->getMaxCasillasMovidas();
            equipoFicha = piezaSeleccionada->getEquipo();

            // dibujar tarjeta de animal seleccionado
            dibujarSprite("../assets/Sprites/tarjetas/tarjetas.png", 256 + deltaTamanoTarjeta, 512 + deltaTamanoTarjeta/2.0f, 69 + (342 * equipoFicha), 32, -5.5, 8, 2, equipoFicha, piezaSeleccionada->getEspecie());
        }

            for (int i = 0; i < Constantes::FILAS_TABLERO; i++) {
                for (int j = 0; j < Constantes::COLUMNAS_TABLERO; j++) {

                    if (abs(i - filaCursor) + abs(j - colCursor) <= maxMovimientos) // solo si es alcanzable
                    {
                        if (equipoFicha == jugadorActivo->getEquipo()) // solo si es mi animal
                        {
                            const Animal* animalDestino = tablero->getAnimalEnCasilla(i, j);

                            // EVALUACION DE CORTOCIRCUITO: solo si esta vacia o animal contrario
                            if (animalDestino == nullptr || animalDestino->getEquipo() != jugadorActivo->getEquipo())
                            {
                                int posPosibleX = 141 + 11 + 22 * j;
                                int posPosibleY = 36 + 11 + 22 * (8 - i);
                                dibujarSprite("../assets/Sprites/tablero/casillaPosible.png", 32, 32, posPosibleX, posPosibleY, -2.5);
                            }
                        }
                    }
                }
            }
        }
    }

    // DIBUJAR LOS ANIMALES EN EL TABLERO
    for (int i = 0; i < Constantes::FILAS_TABLERO; i++) {
        for (int j = 0; j < Constantes::COLUMNAS_TABLERO; j++) {
            const Animal* animal = tablero->getAnimalEnCasilla(i, j);
            if (animal != nullptr) {
                this->dibujar(animal);
            }
        }
    }
    
    // DIBUJAR LOS ANIMALES MUERTOS
    for (Animal* muerto : tablero->getPiezasMuertas()) {
        this->dibujar(muerto); 
    }

    // DIBUJAR LA PIEZA AGARRADA
    if (jugadorActivo->tienePiezaAgarrada()) {
        this->dibujar(jugadorActivo->getPiezaSeleccionada());
    }

    // DIBUJAR EL CURSOR
    // falta que haya dos cursores de diferentes colores, uno para cada jugador
    dibujarSprite("../assets/Sprites/tablero/cursor.png", 64, 32, cursorActivo.getPosX(), cursorActivo.getPosY(), -5.0f,1,2, cursorActivo.getIdJugador(),0);

    // DIBUJAR AL PATO jajaja
    dibujarSprite("../assets/Sprites/tablero/pato.png", 128, 64, tablero->pato.posicion.x, tablero->pato.posicion.y, -5.0f, 2, 4, tablero->pato.frameActualX_, tablero->pato.frameActualY_);

}

//MENU
void Renderizador::dibujar(const Menu* menu) const
{
    float ancho = menu->getAncho();
    float alto = menu->getAlto();

    // FONDO
    dibujarSprite("../assets/Sprites/menu/fondo.png", 512, 512, ancho / 2, alto / 2, 0);

    // NUBES
    for (int i = 0; i < menu->getNumeroNubes(); i++) {
        // referencia constante para no copiar toda la estructura
        const Nube& n = menu->getNube(i);
        dibujarSprite("../assets/Sprites/menu/nube2.png", 512, 128, n.posx, n.posy, n.capa);
    }

    // GRANERO Y VALLAS
    dibujarSprite("../assets/Sprites/menu/granerovallas.png", 512, 512, ancho / 2, alto / 2, -1.9);

    // LETRAS
    for (int i = 0; i < 6; i++) {
        const Letra& l = menu->getLetra(i);
        const std::string ruta = std::string("../assets/Sprites/menu/titulo") + std::to_string(i) + ".png";
        dibujarSprite(ruta.c_str(), 64, 64, 0.1625 * ancho + l.horiz, l.altura, l.capa);
    }

    // obtener las estructuras completas para leer sus datos
    const Selector& sel = menu->getSelector();
    const Paloma& paloma = menu->getPaloma();
    const Tractor& tractor = menu->getTractor();

    // OPCIONES
    dibujarSprite("../assets/Sprites/menu/opciones.png", 512, 128, 245, 103, -2.8, 1, 4, sel.opcionActual, 0);

    // SELECTOR
    dibujarSprite("../assets/Sprites/menu/selector.png", sel.tamano_actual, sel.tamano_actual, sel.posx, sel.posy, -3.2);

    // PALOMA
    dibujarSprite("../assets/Sprites/menu/palomaSpritesheet.png", 128, 64, paloma.posx, paloma.posy, -3, 2, 4, paloma.frameActualX_, paloma.frameActualY_);

    // TRACTOR
    dibujarSprite("../assets/Sprites/menu/tractorSpritesheet.png", 512, 128, tractor.posx, tractor.posy, -5, 1, 2, tractor.frameActualX_, tractor.frameActualY_);
}

void Renderizador::dibujar(const Controles* controles) const
{
    // DIBUJAR FONDO
    dibujarSprite("../assets/Sprites/controles/fondo.png", 3 * 512, 3 * 128, controles->getPosX(), controles->getPosY(), -1);

    // DIBUJAR PALOMA
    // referencia constante a la paloma para leer su estado sin copiar la estructura
    const PalomaControles& paloma = controles->getPaloma();

    dibujarSprite("../assets/Sprites/controles/palomaControles.png", 3 * 256, 3 * 128,
        paloma.getPosX(), paloma.getPosY(), -2, 4, 8,
        paloma.getFrameActualX(), paloma.getFrameActualY());
}

void Renderizador::dibujar(const Creditos* creditos) const
{
    // DIBUJAR FONDO CRÉDITOS
    dibujarSprite("../assets/Sprites/creditos/creditos.png", 2048, 512,
        creditos->getPosX(), creditos->getPosY(), -1);

    // DIBUJAR BOTELLA
    const BotellaCreditos& botella = creditos->getBotella();
    dibujarSprite("../assets/Sprites/creditos/botella.png", 3 * 256, 3 * 32,
        botella.getPosX(), botella.getPosY(), -1.9, 1, 8,
        botella.getFrameActualX(), botella.getFrameActualY());

    // DIBUJAR GALLINA
    const GallinaCreditos& gallina = creditos->getGallina();
    dibujarSprite("../assets/Sprites/creditos/gallinaCreditos.png", 3 * 256, 3 * 32,
        gallina.getPosX(), gallina.getPosY(), -2, 1, 8,
        gallina.getFrameActualX(), gallina.getFrameActualY());
}

void Renderizador::dibujar(const Transicion* transicion) const
{
    if (!transicion->getActivo()) return; // este if antes se hacía en juego.cpp, 
    // pero como el renderizador es el único que sabe dibujar la transición, 
    // me parece más lógico que sea él quien decida no dibujarla si no está activa

    float tamano = transicion->getTamano();

    if (tamano > 0) {
        dibujarSprite("../assets/Sprites/menu/transicionAgujero.png", tamano, tamano, 480 / 2, 270 / 2, -10, 1, 1, 0, 0, false); // agujero transparente
    }

    dibujarSprite("../assets/Sprites/menu/transicionFondo.png", 480, 270, 480 / 2, 270 / 2, -10, 1, 1, 0, 0, false); // fondo negro
}

void Renderizador::dibujar(const Arena* arena) const
{
 
    dibujarSprite("../assets/Sprites/batalla/oficial.png", 512, 512, 480 / 2, 270 / 2, -1);
    //dibujarArena(ARENA_MARGEN_X, ARENA_MARGEN_Y, ZONA_DE_COMBATE_X, ZONA_DE_COMBATE_Y, 0.1f, 0.2f, 0.6f, -5.0f);

    // DIBUJAR BARRERAS
    for (int i = 0; i < NUM_DE_BARRERAS; i++)
    {
        if (arena->isBarreraVisible(i))
        {
            dibujarSprite("../assets/Sprites/batalla/obstaculos.png", 32, 32, arena->getBarreraX(i), arena->getBarreraY(i), -3);
           //dibujarBarreras(arena->getBarreraX(i) - 7, arena->getBarreraY(i) - 9, 14, 18, 0.6f, 0.6f, 0.6f, -3.0f);
           //dibujarBarreras(arena->getBarreraX(i) - 8, arena->getBarreraY(i) - 11, 16, 4, 0.8f, 0.8f, 0.8f, 0.1f);
        }
        else
        {
            //dibujarBarreras(arena->getBarreraX(i) - 6, arena->getBarreraY(i) - 11, 12, 3, 0.2f, 0.2f, 0.2f, -0.9f);
            //dibujarSprite("../assets/Sprites/batalla/obstaculos.png", 32, 32, arena->getBarreraX(i) - 7, arena->getBarreraY(i) - 9, -3);
        }
    }

    // DIBUJAR ATAQUES ACTIVOS
    for (int i = 0; i < 2; i++) {
        const Ataque* atq = arena->getAtaqueObjeto(i);
        
        if (atq && atq->isActivo()) 
            dibujarSprite(atq->getSprite(), atq->getTamanio(), atq->getTamanio(),atq->getX(), atq->getY(),-4.40f,1, 1, 0, 0,true);
  
    }

    // DIBUJAR COMBATIENTES
    for (int i = 0; i < 2; i++) 
        if (arena->isVivo(i)) 
            if (arena->getCombatiente(i) != nullptr) 
                this->dibujar(arena->getCombatiente(i));

    // DIBUJAR VIDA de combatientes
    dibujarSprite("../assets/Sprites/vida/vida.png", 512, 128, 49, 210, -3,1,16, arena->getCombatiente(0)->getVida(), 0);
    dibujarSprite("../assets/Sprites/vida/vida.png", 512, 128, 431, 210, -3, 1, 16, arena->getCombatiente(1)->getVida(), 0);

    // DIBUJAR ATAQUE de combatientes
    dibujarSprite("../assets/Sprites/vida/ataque.png", 512, 128, 22, 210, -3, 1, 16, arena->getCombatiente(0)->getRecargaAtaque(), 0);
    dibujarSprite("../assets/Sprites/vida/ataque.png", 512, 128, 460, 210, -3, 1, 16, arena->getCombatiente(1)->getRecargaAtaque(), 0);
}

void Renderizador::dibujar(const Ganador* ganador) const
{
    // DIBUJAR FONDO Y LETRERO DE TURNOS
    dibujarSprite("../assets/Sprites/pantallaGanador/ganador.png", 512, 512, 480 / 2, 270 / 2, -1);

}

// FUNCION GENERAL PARA DIBUJAR UN SPRITE, SE USA PARA TODO
void Renderizador::dibujarSprite(const char* rutaImagen, float ancho, float alto, float posx, float posy, float capa, int rows, int cols, int stateX, int stateY, bool transparencia) const
{
    float anchoFrame = ancho / cols; // si es solo un frame anchoFrame = ancho pasado en parametro
    float altoFrame = alto / rows;
    float inicioFrameX = stateX * anchoFrame;
    float inicioFrameY = stateY * altoFrame;
  
    glEnable(GL_TEXTURE_2D);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glBindTexture(GL_TEXTURE_2D, ETSIDI::getTexture(rutaImagen).id); // ruta foto

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // mejora calidad
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	if (transparencia) { // si el sprite tiene transparencia, activamos el alpha test
        glEnable(GL_ALPHA_TEST);        // para que no se dibujen los píxeles que sean completamente transparentes, 
        glAlphaFunc(GL_GREATER, 0.05f); // mejora rendimiento y evita artefactos raros entre animales
    }

    glDisable(GL_LIGHTING);
    glColor3f(1, 1, 1);

    glPushMatrix();  // el renderizador se encarga de mover el origen antes de dibujar en el (0,0)
    glTranslatef(posx, posy, capa); // ahora posicion (x,y) en su capa (capa) corresponden a centro de imagen
    glBegin(GL_POLYGON);
    glTexCoord2d(inicioFrameX/ancho               , (inicioFrameY + altoFrame)/alto);    glVertex3f( -anchoFrame/2, -altoFrame/2, 0); // abajo izquierda (0,1)
    glTexCoord2d((inicioFrameX + anchoFrame)/ancho, (inicioFrameY + altoFrame)/alto);    glVertex3f(  anchoFrame/2, -altoFrame/2, 0); // abajo derecha (1,1)
    glTexCoord2d((inicioFrameX + anchoFrame)/ancho, inicioFrameY/alto);                  glVertex3f(  anchoFrame/2,  altoFrame/2, 0); // arriba derecha (1,0)
    glTexCoord2d(inicioFrameX/ancho               , inicioFrameY/alto);                  glVertex3f( -anchoFrame/2,  altoFrame/2, 0); // arriba izquierda (0,0)
    glEnd();
    glPopMatrix();
    
	if (transparencia) {  // desactivamos el alpha test para no afectar a los siguientes sprites que se dibujen
        glDisable(GL_ALPHA_TEST);
    }

    //glEnable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);   
}

