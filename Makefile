#  Gabriel Lüders (GRR20190172)

# ----------------------------------------------------------------- #
# Declaration of targets and compiling options
CFLAGS = -Wall
CC = g++
LIBS = 
OBJ_SERVER = server.o socket.o
OBJ_CLIENT = client.o socket.o
EXEC_SERVER = ./server
EXEC_CLIENT = ./client

# ----------------------------------------------------------------- #
# Compilation directives
all: ${EXEC_SERVER} ${EXEC_CLIENT}

${EXEC_SERVER}: $(OBJ_SERVER)
	$(CC) -o ${EXEC_SERVER} $(OBJ_SERVER) $(LIBS)

${EXEC_CLIENT}: $(OBJ_CLIENT)
	$(CC) -o ${EXEC_CLIENT} $(OBJ_CLIENT) $(LIBS)

server.o: server.cpp socket.cpp
	$(CC) -c server.cpp $(CFLAGS)

client.o: client.cpp socket.cpp
	$(CC) -c client.cpp $(CFLAGS)

socket.o: socket.cpp
	$(CC) -c socket.cpp

runServer: all
	sudo ${EXEC_SERVER}

runClient: all
	sudo ${EXEC_CLIENT}

# ----------------------------------------------------------------- #
# Cleaning directives
clean:
	$(RM) *.o *.gch

purge: clean
	$(RM) ${EXEC_CLIENT} ${EXEC_SERVER} 