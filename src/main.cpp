#include "iostream"
#include "ETSIDI.h" // Biblioteca para sprites y sonidos
#include "freeglut.h" // Biblioteca de ventana, dibujo y manejo teclado
#include "juego.h"

Juego rancho;

// los callback, funciones que seran llamadas automaticamente por la glut cuando sucedan eventos
void OnDraw(void);		 // llamada para dibujar
void OnTimer(int value); // llamada cuando transcurra una temporizacion
void OnKeyboardDown(unsigned char key, int x, int y); // cuando se pulsa una tecla	
void OnKeyboardUp(unsigned char key, int x, int y); // cuando se suelta la tecla
void OnSpecialDown(int key, int x, int y); // cuando se pulsa una tecla	especial
void OnSpecialUp(int key, int x, int y); // cuando se suelta la tecla especial

int main(int argc, char* argv[]) 
{
	std::cout << "Iniciando Rancho...\n";
		
	//Inicializar el gestor de ventanas GLUT y crear la ventana
	glutInit(&argc, argv);
	glutInitWindowSize(rancho.anchoVentana(), rancho.altoVentana());
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutCreateWindow("Rancho");

	//habilitar luces y definir perspectiva
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
	glDisable(GL_LIGHTING);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, rancho.anchoVentana(), 0, rancho.altoVentana(), 50, -50); // alto y ancho podrian hacerse static para acceder desde otras clases como menú (
	glutFullScreen();
	
	//Registrar los callbacks
	glutDisplayFunc(OnDraw);
	glutTimerFunc(25, OnTimer, 0); //le decimos que dentro de 25ms llame 1 vez a la funcion OnTimer()

	glutKeyboardFunc(OnKeyboardDown);  // se pulsa una tecla
	glutKeyboardUpFunc(OnKeyboardUp);  // se suelta la tecla
	glutSpecialFunc(OnSpecialDown);	   // se pulsa una tecla especial
	glutSpecialUpFunc(OnSpecialUp);	   // se suelta la tecla especial

	//pasarle el control a GLUT, que llamara a los callbacks
	glutMainLoop();

	return 0;
}

void OnDraw(void)
{
	rancho.renderizarGraficos();

	glutSwapBuffers();
}

void OnTimer(int value)
{
	rancho.actualizarLogica(25);

	glutTimerFunc(25, OnTimer, 0);
	glutPostRedisplay();
}

void OnKeyboardDown(unsigned char key, int x_t, int y_t)
{
	rancho.procesarTeclaPresionada(key);
}

void OnKeyboardUp(unsigned char key, int x_t, int y_t) 
{
	rancho.procesarTeclaLevantada(key);
}

void OnSpecialDown(int key, int x_t, int y_t)
{
	rancho.procesarTeclaEspecialPresionada(key);
}

void OnSpecialUp(int key, int x_t, int y_t)
{
	rancho.procesarTeclaEspecialLevantada(key);
}

