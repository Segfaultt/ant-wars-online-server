#include "common.h"
#include "entity.h"
#include "maps.h"
#include "network.h"

#include <chrono>
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
      int c_x, c_y, c_z;
};

void player_listener(player *p) {
      uint8_t buff[BUFFER_SIZE];

      while (!p->quit) {
            std::memset(buff, 0, BUFFER_SIZE);
            simple_receive(p->fd, p->addr, buff);
            switch (buff[0]) {}
      }
}

int main(int argc, char *argv[]) {
      // open map
      map map;
      if (argc != 3) {
            std::cout << "Specify map with first argument and number of "
                         "players with the second"
                      << std::endl;
            return -1;
      }
      std::ifstream f(argv[1]);
      if (!f.is_open()) {
            std::cout << "Could not open map" << std::endl;
            return -2;
      }
      map.read_map(f);

      // connect clients
      uint8_t buffer[BUFFER_SIZE];
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
            std::cout << "player " << temp.id << " connected as "
                      << NAME[temp.fact] << std::endl;
      }
      std::vector<std::thread *> listeners;
      listeners.reserve(players.size());
      for (int i = 0; i < players.size(); i++) {
            listeners.push_back(
                new std::thread([=]() { player_listener(players[i]); }));
            std::cout << "started listener thread for player " << players[i]->id
                      << std::endl;

            //---=== set camera ===---
            const int x_init = 0, y_init = 0;
            const float z_init = 1;

            players[i]->c_x = x_init;
            players[i]->c_y = y_init;
            players[i]->c_z = z_init;

            // send set camera packet
            std::memset(buffer, 0, BUFFER_SIZE);
            buffer[0] = 2;
            int a = 1;
            uint32_to_uint8_t(x_init, buffer, a);
            uint32_to_uint8_t(y_init, buffer, a);
            uint32_to_uint8_t(z_init * 1024, buffer, a);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            std::cout << "setting player " << players[i]->id
                      << " camera to (" << x_init << ", " << y_init
                      << ") with zoom: " << z_init << std::endl;
            simple_send(players[i]->fd, players[i]->addr, buffer);

            //---=== set player map ===---
            std::memset(buffer, 0, BUFFER_SIZE);
            buffer[0] = 4;
            std::memcpy((buffer + 1), argv[1], strlen(argv[1]));
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            std::cout << "sending map to player" << players[i]->id
                      << std::endl;
            simple_send(players[i]->fd, players[i]->addr, buffer);
      }

      auto t1 = std::chrono::high_resolution_clock::now();
      auto dt = std::chrono::high_resolution_clock::now() - t1;
      while (true) {
            // testing code
            std::memset(buffer, 0, BUFFER_SIZE);
            buffer[0] = 3;
            int a = 1;
            uint32_to_uint8_t(1, buffer, a);
            uint32_to_uint8_t(NIPPER, buffer, a);
            uint32_to_uint8_t(0, buffer, a);
            uint32_to_uint8_t(0 + 100 * dt.count(), buffer, a);
            uint32_to_uint8_t(2, buffer, a);
            uint32_to_uint8_t(FLYING, buffer, a);
            uint32_to_uint8_t(500, buffer, a);
            uint32_to_uint8_t(500, buffer, a);
            simple_send(players[0]->fd, players[0]->addr, buffer, BUFFER_SIZE);

            // cap loop speed
            if (dt.count() < 10)
                  std::this_thread::sleep_for(std::chrono::milliseconds(10) -
                                              dt);

            // update initial time (t1) and delta time (dt)
            // this should always be last in the game loop
            dt = std::chrono::high_resolution_clock::now() - t1;
            t1 = std::chrono::high_resolution_clock::now();
      }

      return 0;
}
