#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <cstring>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstdlib>
#include <ctime>

class PartidaServidor {
public:
    PartidaServidor(int puerto);
    void ejecutar();

private:
    int socket_servidor;  // Socket del servidor
    struct sockaddr_in direccion_servidor;  //  Dirección del servidor
    std::vector<std::thread> lista_hilos;  // Lista de hilos para atender a los clientes
    std::mutex cerradura;  // Cerradura para evitar condiciones de carrera en la impresión de mensajes

    void atenderCliente(int cliente_socket, const std::string& info_cliente);
    void dibujarTablero(char tablero[6][7]);
    bool comprobarGanador(char tablero[6][7], char jugador);
};

PartidaServidor::PartidaServidor(int puerto) {
    // Crea socket del servidor y verifica si se creó correctamente
    socket_servidor = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_servidor == 0) {
        perror("Error al crear el socket del servidor");
        exit(EXIT_FAILURE);
    }
    
    // Configura la dirección del servidor y verifica si se configuró correctamente
    direccion_servidor.sin_family = AF_INET;
    direccion_servidor.sin_addr.s_addr = INADDR_ANY;
    direccion_servidor.sin_port = htons(puerto);

    // Enlaza el socket a la dirección y puerto especificados.
    if (bind(socket_servidor, (struct sockaddr *)&direccion_servidor, sizeof(direccion_servidor)) < 0) {
        perror("Error al enlazar el socket al puerto especificado");
        exit(EXIT_FAILURE);
    }

    // Escucha conexiones entrantes y verifica si se realizó correctamente
    if (listen(socket_servidor, 3) < 0) {
        perror("Error al escuchar conexiones entrantes");
        exit(EXIT_FAILURE);
    }
}

bool PartidaServidor::comprobarGanador(char tablero[6][7], char jugador) {
    // Verifica victoria horizontal
    for (int fila = 0; fila < 6; ++fila) {
        for (int col = 0; col < 4; ++col) {
            if (tablero[fila][col] == jugador &&
                tablero[fila][col + 1] == jugador &&
                tablero[fila][col + 2] == jugador &&
                tablero[fila][col + 3] == jugador) {
                return true;
            }
        }
    }
    
    // Verifica victoria diagonal 
    //(Desde esquina superior derecha hacia esquina inferior izquierda)
    for (int fila = 3; fila < 6; ++fila) {
        for (int col = 0; col < 4; ++col) {
            if (tablero[fila][col] == jugador &&
                tablero[fila - 1][col + 1] == jugador &&
                tablero[fila - 2][col + 2] == jugador &&
                tablero[fila - 3][col + 3] == jugador) {
                return true;
            }
        }
    }
    // Verifica victoria vertical
    for (int fila = 0; fila < 3; ++fila) {
        for (int col = 0; col < 7; ++col) {
            if (tablero[fila][col] == jugador &&
                tablero[fila + 1][col] == jugador &&
                tablero[fila + 2][col] == jugador &&
                tablero[fila + 3][col] == jugador) {
                return true;
            }
        }
    }
    // Verifica victoria diagonal 
    //(desde esquina superior izquierda hacia esquina inferior derecha)
    for (int fila = 0; fila < 3; ++fila) {
        for (int col = 0; col < 4; ++col) {
            if (tablero[fila][col] == jugador &&
                tablero[fila + 1][col + 1] == jugador &&
                tablero[fila + 2][col + 2] == jugador &&
                tablero[fila + 3][col + 3] == jugador) {
                return true;
            }
        }
    }
    return false;
}
// Imprime el juego 4 en línea
void PartidaServidor::dibujarTablero(char tablero[6][7]) {
    std::cout << "4 en Línea" << std::endl;
    for (int i = 0; i < 6; ++i) {
        for (int j = 0; j < 7; ++j) {
            if (i < 6 && j==0) {
        std::cout << " "<< i+1 << " ";
   		}
            std::cout << tablero[i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "   -------------" << std::endl;
    std::cout << "   1 2 3 4 5 6 7" << std::endl;
}

void PartidaServidor::atenderCliente(int cliente_socket, const std::string& info_cliente) {
    char tablero[6][7] = {};
    memset(tablero, '-', sizeof(tablero));

    // Determina quién comienza el juego de manera aleatoria
    bool turno_cliente = rand() % 2 == 0;
    send(cliente_socket, &turno_cliente, sizeof(turno_cliente), 0);
    std::cout << "Juego [" << info_cliente << "]: " << (turno_cliente ? "EL primer turno es del Cliente" : "El primer turno es del servidor") << std::endl;

    while (true) {
        int columna;
        if (turno_cliente) {
            // Recibe movimiento del cliente
            recv(cliente_socket, &columna, sizeof(columna), 0);
            for (int i = 5; i >= 0; --i) {
                if (tablero[i][columna] == '-') {
                    tablero[i][columna] = 'C';
                    break;
                }
            }
            std::cout << "Juego [" << info_cliente << "]: Cliente juega en la columna " << columna + 1 << "." << std::endl;
        } else {
            // Movimiento del servidor
            columna = rand() % 7;
            for (int i = 5; i >= 0; --i) {
                if (tablero[i][columna] == '-') {
                    tablero[i][columna] = 'S';
                    break;
                }
            }
            std::cout << "Juego [" << info_cliente << "]: Servidor juega en la columna " << columna + 1 << "." << std::endl;
        }

// Imprime tablero y envía tablero actualizado al cliente
        send(cliente_socket, tablero, sizeof(tablero), 0);

        // Verificación de victoria
        if (comprobarGanador(tablero, turno_cliente ? 'C' : 'S')) {
            char estado = 'G';  // 'G' indica que el jugador actual ha ganado
            send(cliente_socket, &estado, 1, 0);
            std::cout << "Juego [" << info_cliente << "]: " << (turno_cliente ? "Ganador: CLIENT" : "Ganador: SERVER") << std::endl;
            break;
        } else {
            // Verificador si hay empate
            bool empate = true;
            for (int i = 0; i < 7; ++i) {
                if (tablero[0][i] == '-') {
                    empate = false;
                    break;
                }
            }
            if (empate) {
                char estado = 'D';  // 'D' indica empate
                send(cliente_socket, &estado, 1, 0);
                std::cout << "Juego [" << info_cliente << "]: Empatado." << std::endl;
                break;
            }
        }

        // Continuar el juego
        char estado = 'C';  // 'C' indica continuar
        send(cliente_socket, &estado, 1, 0);
        turno_cliente = !turno_cliente;
    }

    // Cierra el socket del cliente y muestra mensaje de finalización
    close(cliente_socket);
    std::cout << "Juego [" << info_cliente << "]: GAME OVER." << std::endl;
}

void PartidaServidor::ejecutar() {
    std::cout << "Servidor en espera de conexiones..." << std::endl;
    srand(time(0));

    while (true) {
        int longitud_direccion = sizeof(direccion_servidor);
        int cliente_socket = accept(socket_servidor, (struct sockaddr *)&direccion_servidor, (socklen_t*)&longitud_direccion);
        if (cliente_socket < 0) {
            perror("Error de conexión");
            continue;
        }

        // Obtiene información del cliente (dirección IP y puerto)
        std::string info_cliente = inet_ntoa(direccion_servidor.sin_addr) + std::string(":") + std::to_string(ntohs(direccion_servidor.sin_port));
        std::cout << "Juego nuevo [" << info_cliente << "]" << std::endl;

        // Crea un hilo para atender al cliente y lo agrega a la lista de hilos
        lista_hilos.push_back(std::thread(&PartidaServidor::atenderCliente, this, cliente_socket, info_cliente));
    }
}


int main(int argc, char const *argv[]) {
    if (argc != 2) {
        std::cerr << "Uso: ./servidor <puerto>" << std::endl;
return -1;
    }

    int puerto = std::stoi(argv[1]);
    PartidaServidor servidor(puerto);
    servidor.ejecutar();

    return 0;
}