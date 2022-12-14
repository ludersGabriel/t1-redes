#  Gabriel Lüders (GRR20190172)

# ----------------------------------------------------------------- #
# Declaration of targets and compiling options
CFLAGS = -Wall -g -std=c++17
CC = g++
LIBS = -lstdc++fs 
OBJ_SERVER = serverMain.o socket.o message.o network.o server.o
OBJ_CLIENT = clientMain.o socket.o message.o network.o client.o utils.o
EXEC_SERVER = ./server
EXEC_CLIENT = ./client

# ----------------------------------------------------------------- #
# Compilation directives
all: ${EXEC_SERVER} ${EXEC_CLIENT}

${EXEC_SERVER}: $(OBJ_SERVER)
	$(CC) -o ${EXEC_SERVER} $(OBJ_SERVER) $(LIBS)

${EXEC_CLIENT}: $(OBJ_CLIENT)
	$(CC) -o ${EXEC_CLIENT} $(OBJ_CLIENT) $(LIBS)

serverMain.o: serverMain.cpp socket.cpp 
	$(CC) -c serverMain.cpp $(CFLAGS) $(LIBS)

clientMain.o: clientMain.cpp socket.cpp utils.h
	$(CC) -c clientMain.cpp $(CFLAGS) $(LIBS)

socket.o: socket.cpp
	$(CC) -c socket.cpp $(CFLAGS)

client.o: client.cpp client.h utils.h
	$(CC) -c client.cpp $(CFLAGS)

utils.o: utils.cpp utils.h
	$(CC) -c utils.cpp $(CFLAGS)

server.o: server.cpp server.h
	$(CC) -c server.cpp $(CFLAGS)

message.o: message.cpp message.h
	$(CC) -c message.cpp $(CFLAGS)

network.o: network.cpp network.h
	$(CC) -c network.cpp $(CFLAGS)

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