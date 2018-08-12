#include "common.h"
#include "network.h"
#include "entity.h"
#include "maps.h"

#include <fstream>
#include <mutex>
#include <thread>

struct player {
	sockaddr_in addr;
	socklen_t socklen;
	int fd;
	int id;
	faction fact;
	bool quit = false;
};

void player_listener(player *p)
{
	while (!p->quit) {

	}
}

int main (int argc, char* argv[])
{
	//open map
	map map;
	if (argc != 3) {
		std::cout << "Specify map with first argument and number of players with the second" << std::endl;
		return -1;
	}
	std::ifstream f(argv[1]);
	if (!f.is_open()) {
		std::cout << "Could not open map" << std::endl;
		return -2;
	}
	map.read_map(f);

	//connect clients
	char buffer[BUFFER_SIZE];
	std::vector<player *> players;
	std::cout << "Ready to connect to clients" << std::endl;
	for (int i = 0; i < atoi(argv[2]); i++) {
		player temp;
		receiver_init(temp.fd, temp.addr);
		temp.id = i + 1;
		do {
			simple_receive(temp.fd, temp.addr, buffer);
		} while (buffer[0] != 1);
		temp.fact = (faction)buffer[1];
		players.push_back(&temp);
		std::cout << "player " << temp.id << " connected as " << NAME[temp.fact] << std::endl;
	}

	std::vector<std::thread *> listeners;
	listeners.reserve(players.size());
	for (int i = 0; i < players.size(); i++) {
		listeners.push_back(new std::thread([=]() {
						player_listener(players[i]);
					}));
		std::cout << "started listener thread for player " << players[i]->id << std::endl;
	}

	return 0;
}
