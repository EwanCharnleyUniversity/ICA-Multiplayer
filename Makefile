CXXFLAGS=-std=c++14
CFLAGS=-g -lsfml-graphics -lsfml-window -lsfml-system -lsfml-network -pthread
CPPFLAGS=
LDFLAGS=
LDLIBS=
LIBS=
CPP=g++

all: main

main: main.o entity.o tile.o builder.o world.o game.o network.o message.o myserver.o receiver.o accepter.o
	$(CPP) $(CXXFLAGS) $(LIBS) $^ -o $@ $(CFLAGS)

server: server.o accepter.o receiver.o myserver.o
	$(CPP) $(CXXFLAGS) $(LDFLAGS) $(LIBS) $^ -o $@ $(CFLAGS)

client: client.o receiver.o myserver.o accepter.o
	$(CPP) $(CXXFLAGS) $(LDFLAGS) $(LIBS) $^ -o $@ $(CFLAGS)



clean:
	\rm -f *.o main
