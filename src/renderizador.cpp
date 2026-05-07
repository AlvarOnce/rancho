#include "renderizador.h"

void Renderizador::limpiarPantalla() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void Renderizador::dibujarSprite(const char* rutaImagen, float ancho, float alto, float posx, float posy, float capa, int rows, int cols, int stateX, int stateY, bool transparencia) {

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