CC = g++

C_FLAGS = -w -std=c++11 -g

L_FLAGS = -pthread

OBJ_NAME = ant_wars_colonial_server

OBJS = obj/main.o obj/entity.o obj/network.o obj/maps.o

all: $(OBJS)
	$(CC) $(L_FLAGS) $(OBJS) -o $(OBJ_NAME)

obj/entity.o: src/entity.cpp
	$(CC) $(C_FLAGS) -c src/entity.cpp -o obj/entity.o

obj/main.o: src/main.cpp
	$(CC) $(C_FLAGS) -c src/main.cpp -o obj/main.o

obj/network.o: src/network.cpp
	$(CC) $(C_FLAGS) -c src/network.cpp -o obj/network.o

obj/maps.o: src/maps.cpp
	$(CC) $(C_FLAGS) -c src/maps.cpp -o obj/maps.o

clean:
	rm $(OBJS)
