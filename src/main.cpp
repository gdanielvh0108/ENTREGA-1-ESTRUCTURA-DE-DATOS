#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include "Juego.h"
#include "GestorArchivo.h"
#include "Color.h"
#include "Condicion.h"

const std::string ARCHIVO_PARTIDA = "partida.txt";

void limpiarEntrada() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int pedirEntero(const std::string& mensaje, int minimo, int maximo) {
    int valor;
    while (true) {
        std::cout << mensaje;
        if (std::cin >> valor && valor >= minimo && valor <= maximo) {
            limpiarEntrada();
            return valor;
        }
        std::cout << "Entrada invalida, intenta de nuevo.\n";
        limpiarEntrada();
    }
}

// Muestra el menu de un turno y devuelve true si el jugador decidio guardar y salir
bool turnoJugador(Juego& juego, int indiceJugador) {
    Jugador& jugador = juego.obtenerJugador(indiceJugador);

    std::cout << "\n--- Turno de " << jugador.obtenerNombre() << " ---\n";
    std::cout << "Tu mano:\n";
    jugador.mostrarCartas();

    std::cout << "1. Jugar una carta\n";
    std::cout << "2. Guardar partida y salir\n";
    int opcion = pedirEntero("Elige una opcion: ", 1, 2);

    if (opcion == 2) {
        GestorArchivo gestor(ARCHIVO_PARTIDA);
        gestor.guardarPartida(juego);
        return true;
    }

    int indiceCarta = pedirEntero("Indice de la carta a jugar: ", 0, jugador.cantidadCartas() - 1);
    juego.jugarTurno(indiceJugador, indiceCarta);
    return false;
}

int main() {
    Juego juego;
    GestorArchivo gestor(ARCHIVO_PARTIDA);
    bool salir = false;

    if (gestor.existePartida()) {
        std::cout << "Se encontro una partida guardada.\n";
        std::cout << "1. Continuar partida guardada\n";
        std::cout << "2. Iniciar partida nueva (se perdera la guardada)\n";
        int opcion = pedirEntero("Elige una opcion: ", 1, 2);

        if (opcion == 1) {
            if (!gestor.cargarPartida(juego)) {
                std::cout << "No se pudo leer el archivo guardado. Se iniciara una nueva partida.\n";
                opcion = 2;
            } else {
                std::cout << "Partida cargada. Continuando en la ronda "
                          << juego.obtenerRondaActual() << " de " << juego.obtenerTotalRondas() << ".\n";
            }
        }

        if (opcion == 2) {
            int numJugadores = pedirEntero("Numero de jugadores: ", 2, 6);
            std::vector<std::string> nombres;
            for (int i = 0; i < numJugadores; i++) {
                std::cout << "Nombre del jugador " << (i + 1) << ": ";
                std::string nombre;
                std::getline(std::cin, nombre);
                nombres.push_back(nombre);
            }
            juego = Juego(nombres, 5);
            juego.iniciarJuego();
        }
    } else {
        int numJugadores = pedirEntero("Numero de jugadores: ", 2, 6);
        std::vector<std::string> nombres;
        for (int i = 0; i < numJugadores; i++) {
            std::cout << "Nombre del jugador " << (i + 1) << ": ";
            std::string nombre;
            std::getline(std::cin, nombre);
            nombres.push_back(nombre);
        }
        juego = Juego(nombres, 5);
        juego.iniciarJuego();
    }

    // --- Bucle principal del juego ---
    while (!salir && !juego.hayJuegoTerminado()) {
        std::cout << "\n========== RONDA " << juego.obtenerRondaActual()
                  << " / " << juego.obtenerTotalRondas() << " ==========\n";

        int iniciador = juego.obtenerJugadorInicial();

        // 1. El iniciador declara la condicion (si aun no se establecio, ej. al cargar)
        if (!juego.condicionYaEstablecida()) {
            std::cout << juego.obtenerJugador(iniciador).obtenerNombre()
                      << " inicia la ronda y debe declarar la condicion.\n";
            std::cout << "Color pedido -> 1. AMARILLO  2. NARANJA\n";
            int colorOp = pedirEntero("Opcion: ", 1, 2);
            Color color = (colorOp == 1) ? Color::AMARILLO : Color::NARANJA;

            std::cout << "Direccion -> 1. BAJO  2. ALTO\n";
            int tipoOp = pedirEntero("Opcion: ", 1, 2);
            TipoCondicion tipo = (tipoOp == 1) ? TipoCondicion::BAJO : TipoCondicion::ALTO;

            juego.establecerCondicion(color, tipo);

            std::cout << "Condicion declarada: " << colorToString(color)
                      << " " << tipoCondicionToString(tipo) << "\n";
        } else {
            std::cout << "Condicion vigente: " << colorToString(juego.obtenerColorCondicion())
                      << " " << tipoCondicionToString(juego.obtenerTipoCondicion()) << "\n";
        }

        // 2. Cada jugador que aun no ha jugado, juega en orden a partir del iniciador
        int n = juego.obtenerNumJugadores();
        for (int paso = 0; paso < n && !salir; paso++) {
            int indice = (iniciador + paso) % n;
            if (juego.jugadorYaJugo(indice)) continue; // ya jugo (ej. tras cargar partida)

            salir = turnoJugador(juego, indice);
        }

        if (salir) break; // se guardo y se sale sin terminar la ronda

        // 3. Determinar ganador de la ronda
        int ganador = juego.determinarGanador();

        if (ganador == -1) {
            std::cout << "\nNadie jugo el color pedido. La ronda se repite.\n";
            juego.repetirRonda();
            continue;
        }

        std::cout << "\nGanador de la ronda: " << juego.obtenerJugador(ganador).obtenerNombre() << "\n";
        juego.avanzarRonda(ganador);
    }

    if (!salir) {
        juego.mostrarResultado();
    } else {
        std::cout << "\nPartida guardada. Puedes cerrar el programa y continuar mas tarde.\n";
    }

    return 0;
}
