# Guía de Estilo - Proyecto Rancho

Este documento establece las convenciones de código basadas en Google C++ Style Guide para garantizar la calidad, legibilidad y estabilidad del proyecto.

## 1. Convenciones de Nomenclatura (Naming)

* **Clases y Estructuras (`PascalCase`):** La primera letra de cada palabra en mayúscula.
  * *Ejemplo:* `class Tablero`, `struct CoordenadaArena`
* **Métodos y Funciones (`camelCase`):** La primera letra en minúscula, las siguientes palabras empiezan en mayúscula.
  * *Ejemplo:* `void actualizarEstado()`, `bool movimientoValidoTablero()`
* **Variables Locales y Parámetros (`snake_case`):** Todo en minúsculas, separando palabras con guiones bajos.
  * *Ejemplo:* `int vida_actual`, `float posicion_x`
* **Atributos de Clase / Miembros Privados (`snake_case_`):** Terminan con un guion bajo para diferenciarlos rápidamente de las variables locales.
  * *Ejemplo:* `Tablero* tablero_juego_;`, `int dano_base_;`
* **Constantes y Enums (`SCREAMING_SNAKE_CASE`):** Todo en mayúsculas con guiones bajos.
  * *Ejemplo:* `MAX_ANIMALES`, `enum class EstadoJuego { MENU, TABLERO, BATALLA }`
* **Ramas (`feature/kebab-case`):** Todo en minúsculas, separando palabras con guiones medios. Empieza con `feature/`
  * *Ejemplo:* `feature/tablero`, `feature/entradas-por-teclado`

## 2. Formato y Sintaxis

* **Llaves `{ }`:** Ambas llaves, la de apertura y cierre van en una nueva línea.
* **Sangría:** 4 espacios por cada nivel de indentación.
* **Punteros y Referencias:** El `*` o el `&` se escriben pegados al tipo de dato (`Animal* gallina;`).

## 3. Arquitectura y C++ Orientado a Objetos

* **Protección:** Todo archivo `.h` debe comenzar con `#pragma once`.
* **Encapsulamiento:** Atributos siempre `private` o `protected`. Por defecto al principio de la clase, donde no hace falta escribir `private`. Crear los métodos `get()` y `set()` solo si es necesario. Poner `set()` en los setters pero no poner `get()` en los getters.
* **Const:** Etiqueta con la palabra `const` todos los métodos que lean datos pero no modifiquen los atributos de la clase (ej. `int getVida() const;`).
* **Memoria:** Todo `new` debe tener su `delete` en el destructor.
