#pragma once
#include "animal.h"  
#include "cursor.h" 
#include "tarjetaID.h" 

const int BANDO_LUZ = 1;        //bando luz igual bando izquierda
const int BANDO_OSCURIDAD = 2;
const int CASILLA_LUZ = 1;
const int CASILLA_OSCURA = 2;

class Tablero {

    Animal* casillas[9][9]; //crea la matriz que es el tablero, cada casilla es un puntero a animal, es decir que tiene los atributos de animal
	int color_casilla[9][9];  //la casilla de abajo a la izquierda es (0,0) y la de arriba a la derecha es (8,8) la matriz de arriba
    bool hay_pieza_seleccionada_ = FALSE;  //1 hay una pieza seleccionada 0 no hay
	Animal* animal_seleccionado_ = nullptr; //puntero al animal seleccionado, si no hay ninguno seleccionado es nullptr
    int turno_actual;

    /*
        Haremos que ColorCasilla sea como una bandera
        Casilla luz = 1
        Casilla Oscuridad = 2
    */
  
    static const int TAMANO_CASILLA = 22;

    static const int FILAS = 9;
    static const int COLUMNAS = 9;

    int contador_turno = 0;

    static const int X_INICIO = 141;
    static const int Y_INICIO = 36;

    /*
        Colocación de neustro tablero en nuestra panatalla
        tomando en cuenta que nuestro tablero es 480x270
        y viendo que nuestras casillas son de 22x22 píxeles
        X_incio=(480-[22x9])=(480-198)/2=141
        Y_incio=(270-[22x9])=(270-198)/2=36
        pd:dividimos entre dos para que cuando se dibuje el tablero este centrado en la ventana emergente.
    */

public:

    Tablero(Animal** misAnimales);//le metemos un puntero a un array de punteros, los animales se crean desde juego y los enviamos
    // a este constructor que simplemente copia la direccion de los animales creados en las casillas del tablero y entonces
    //cada casilla apunta a un animal creado y tiene toda la informacion de ese animal y puede modificar valores de el
    ~Tablero();//no hace nada porque el constructor no tiene ningun new


    void inicializarTablero(); //pone el color de las casillas del tablero y pone que no haya animales
    void recibirMovimiento(int jugador, int dx, int dy);  //recibe la tecla pulsada y la procesa
    void seleccionarPieza(int jugador);    //identifica si en la casilla hay un animal, si lo hay, bloquea el cursor y tocaria que se moviese el animal y no el cursor
	void actualizar(float dt);     //actualiza el estado del tablero, se llama en cada iteracion del juego, se encarga de actualizar los animales y el cursor
    void dibujar(Renderizador* motor);

    Cursor cursor;
    Tarjeta tarjeta;
}; 

//actualmente el tablero se puede mover con awsd porque es el turno del jugador 1, si fuese del 2 con las flechas, si pulsas . el 
// programa mira si hay una pieza en la casilla, y si la hay bloquea el cursor. El siguiente paso es que despues de bloquear el
// cursor, awsd mueva al animal. Otras cosas para hacer son:
//me gustaria pero es muy complicado, que no se pueda mover nada hasta que no 
// termine la animacion de los animales moverse a su sitio, arreglar el bug de que la d y la flecha izquierda son iguales