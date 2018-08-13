#pragma once 

#include <iostream>
#include <ctime>
#include <chrono>
#include <math.h>
#include <vector>
#include <cstring>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

enum faction {
	GERMANIC,
	ROMAN,
	SOVIET,
	LAST_FACTION
};

const std::string FONT[] = {"res/Autobahn.ttf", "res/Roman SD.ttf", "res/Red October.ttf"},
	NAME[] = {"German Reich", "SPQR", "Soviet Socialist Union"};

const int PORT_CLIENT = 52757,
	PORT_SERVER = 52657;

const unsigned int BUFFER_SIZE = 511;

const std::string VERSION = "s0.0";

const double PI = 3.14159265359;

/*packet rules*/
/* 0XXXX - network info
   06000 - ACK packet
   1FXXX - client joining (F - theme/faction)
   2 x y z 0 - 32 bit - set camera (x - x coord, y - y coord, z - zoom * 1024) 
   3 i e x y a h s - 32 bit - repeating - visible entity (i - id, e - entity type, x - x coord, y - y coord, a - angle in radians * 1024, h - health, s - stamina)
 */

const uint8_t ACK[] = {0, 6};
