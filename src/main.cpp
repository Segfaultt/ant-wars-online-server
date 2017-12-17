#include "common.h"
#include "network.h"
#include "entity.h"
#include "maps.h"
#include <fstream>

struct player {
	sockaddr_in addr;
	int sockfd;
	int id;
	faction fact;
};

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
	int sockfd_r;
	sockaddr_in sock_addr_r;
	char buffer[BUFFER_SIZE];
	receiver_init(sockfd_r, sock_addr_r);
	std::vector<player> players;
	std::cout << "Ready to connect to clients" << std::endl;
	for (int i = 0; i < atoi(argv[2]); i++) {
		player temp;
		temp.id = i + 1;
		simple_receive(sockfd_r, sock_addr_r, buffer);
		temp.fact = (faction)buffer[0];
		players.push_back(temp);
		std::cout << "player " << temp.id << " connected as " << NAME[temp.fact] << std::endl;
	}
       
	return 0;
}
