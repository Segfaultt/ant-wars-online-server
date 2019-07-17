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
      visual_entity ant;
      bool quit = false;
      int c_x, c_y, c_z;
};

void player_listener(player *p) {
      uint8_t buff[BUFFER_SIZE];

      while (!p->quit) {
            std::memset(buff, 0, BUFFER_SIZE);
            simple_receive(p->fd, p->addr, buff);
            switch (buff[0]) {
            case 5: // move forward temp
                  const int speed = 100;
                  switch (buff[1]) {
                  case 0:
                        p->ant.x += speed * cos(p->ant.angle * PI / 180);
                        p->ant.y += speed * sin(p->ant.angle * PI / 180);
                        break;

                  case 1:
                        p->ant.x -= speed * cos(p->ant.angle * PI / 180);
                        p->ant.y -= speed * sin(p->ant.angle * PI / 180);
                        break;

                  case 2:
                        p->ant.angle += 5;
                        p->ant.angle = (p->ant.angle + 360) % 360;
                        break;

                  case 3:
                        p->ant.angle -= 5;
                        p->ant.angle = (p->ant.angle + 360) % 360;
                        break;
                  }
                  break;
            }
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
            temp.ant.et = (entity_type)buffer[1];
            temp.ant.id = temp.id;
            temp.ant.health = 100;
            temp.ant.stamina = 100;
            temp.ant.x = 0;
            temp.ant.y = 0;
            players.push_back(&temp);
            std::cout << "player " << temp.id << " connected as " << temp.ant.et
                      << std::endl;
      }
      std::vector<std::thread *> listeners;
      listeners.reserve(players.size());
      for (int i = 0; i < players.size(); i++) {
            listeners.push_back(
                new std::thread([=]() { player_listener(players[i]); }));
            std::cout << "started listener thread for player " << players[i]->id
                      << std::endl;

            // send visual entity ID
            std::memset(buffer, 0, BUFFER_SIZE);
            buffer[0] = 2;
            int a = 1;
            uint32_to_uint8_t(i, buffer, a);
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            std::cout << "Telling player " << players[i]->id
                      << " their visual entity ID" << std::endl;
            simple_send(players[i]->fd, players[i]->addr, buffer);

            //---=== set player map ===---
            std::memset(buffer, 0, BUFFER_SIZE);
            buffer[0] = 4;
            std::memcpy((buffer + 1), argv[1], strlen(argv[1]));
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            std::cout << "sending map to player" << players[i]->id << std::endl;
            simple_send(players[i]->fd, players[i]->addr, buffer);
      }

      auto t1 = std::chrono::high_resolution_clock::now();
      auto dt = std::chrono::high_resolution_clock::now() - t1;
      double count = 0;
      while (true) {
            // send render list
            std::memset(buffer, 0, BUFFER_SIZE);
            buffer[0] = 3;
            int a = 1;

            for (auto player : players) {
                  player->ant.add_to_buff(buffer, a);
            }
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
