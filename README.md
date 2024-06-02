Juego 4 en línea

"4 en línea" (también conocido como "Conecta 4") es un juego de mesa para dos jugadores en el que se turnan para dejar caer discos de colores en una rejilla vertical de siete columnas y seis filas. El objetivo es ser el primero en alinear cuatro de tus propios discos consecutivos en cualquier dirección: horizontal, vertical o diagonal. Los jugadores deben planear sus movimientos estratégicamente para conectar cuatro discos mientras bloquean a su oponente de hacer lo mismo. El juego termina cuando uno de los jugadores logra alinear cuatro discos o cuando la rejilla está llena sin un ganador, resultando en un empate. En esta tarea de programación, tuve que realizar el juego en C++ con el modelo de Cliente-Servidor.

Instrucciones de compilación y ejecución

1. Abre una terminal.
2. Clona el repositorio con el siguiente comando:
``` git clone https://github.com/LeonardoGutierrezS/Tarea1ComunicacionRedesyDatos.git ```
3. Cambia al directorio del proyecto con el siguiente comando:
``` cd Tarea1ComunicacionRedesyDatos ```
4. Compila el servidor con el siguiente comando:
``` make ```
5 para eliminar los archivos generados por el comando make, ejecuta el siguiente comando:

```bash 
    make clean ```
6. Ejecuta el servidor con el siguiente comando (reemplaza <puerto> por el puerto que desees):
``` ./server <puerto> ```
7. Compila el cliente con el siguiente comando:
``` make client ```
8. Ejecuta el cliente con el siguiente comando (reemplaza <ip> y <puerto> por la dirección IP y puerto del servidor, respectivamente):
``` ./client <ip> <puerto> ```
9. Sigue las instrucciones en pantalla para jugar "4 en línea" con el servidor.

Instrucciones de uso del juego.

1. El servidor debe ejecutarse antes que el cliente.
2. El servidor debe ejecutarse con un puerto como argumento.
3. El cliente debe ejecutarse con la dirección IP y puerto del servidor como argumentos.
4. El cliente debe conectarse al servidor antes de poder jugar.
5. El cliente debe esperar su turno para hacer un movimiento.
6. El cliente debe ingresar un número de columna válido para dejar caer un disco.