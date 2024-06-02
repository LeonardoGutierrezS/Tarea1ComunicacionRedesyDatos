# Construir ambos objetivos por defecto
all: Server Client

# Compilar 'Server' con soporte para hilos (pthread)
Server: Server.cpp
	g++ -o Server Server.cpp -pthread

# Compilar 'Client'
Client: Client.cpp
	g++ -o Client Client.cpp

# Eliminar archivos generados por la compilación
clean:
	rm -f servidor cliente