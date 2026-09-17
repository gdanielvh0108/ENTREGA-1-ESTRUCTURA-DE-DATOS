# 🎴 Juego de Cartas: Alto/Bajo por Color

Juego de cartas por turnos implementado en *C++*, donde cada jugador compite para cumplir la condición (color e intensidad) impuesta por el jugador que inicia cada ronda.

## 📜 Descripción del juego

- Se juega con un número variable de jugadores (definido al iniciar la partida).
- Cada jugador recibe *6 cartas: 3 de color **Amarillo* y 3 de color *Naranja, cada una con una **intensidad* entre 1 y 10.
- Un jugador aleatorio inicia la primera ronda.

### Mecánica de una ronda

1. El *jugador iniciador* declara una *condición: un color (Amarillo/Naranja) y una dirección (Alto/Bajo). Ejemplo: *"Amarillo, bajito".
2. El iniciador juega una carta de su mano.
3. Los demás jugadores, en orden de turno, juegan una carta de su mano (pueden elegir cualquiera, pero si no coincide con el color pedido, *no pueden ganar* esa ronda).
4. Se determina el ganador: entre quienes jugaron el color pedido, gana quien tenga la intensidad *más baja* (si la condición fue "Bajo") o *más alta* (si fue "Alto").
5. *Caso especial:* si ningún jugador (ni siquiera el iniciador) jugó el color pedido, la ronda se *repite* — las cartas jugadas vuelven a la mano de sus dueños y el mismo iniciador vuelve a declarar condición.
6. El ganador de la ronda recibe todas las cartas jugadas por los demás jugadores y se convierte en el iniciador de la siguiente ronda.

### Fin del juego

- Se juegan un total de *5 rondas*.
- Cada carta en posesión de un jugador vale *1 punto*.
- Gana la partida quien termine con más cartas (más puntos) al finalizar la ronda 5.

## 🏗️ Estructura del proyecto


## 📐 Diagrama UML


<img width="1536" height="1024" alt="d1923efb-d915-4f9a-96b3-d438a463f09d" src="https://github.com/user-attachments/assets/0f2dc3de-3783-4a90-a859-d1686f99d63e" />


