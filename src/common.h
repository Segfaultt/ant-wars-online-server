#pragma once

#include <arpa/inet.h>
#include <chrono>
#include <cstring>
#include <ctime>
#include <iostream>
#include <math.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <vector>

const std::string font = "res/Roman SD.ttf";

const int PORT_CLIENT = 52390, PORT_SERVER = 52053;

const unsigned int BUFFER_SIZE = 511;

const std::string VERSION = "s0.0";

const double PI = 3.14159265359;

/*packet rules*/
/* 0XXXX - network info
   06000 - ACK packet
   1FXXX - client joining (F - theme/faction)
   2 x y z 0 - 32 bit - set camera (x - x coord, y - y coord, z - zoom * 1024)
   3 i e x y a h s - 32 bit - repeating - visible entity (i - id, e - entity
   type, x - x coord, y - y coord, a - angle in radians * 1024, h - health, s -
   stamina)
   4 d 0 X X - tell client what map is being used (d - c string showing the
   location of the map image)
 */

const uint8_t ACK[] = {0, 6};
