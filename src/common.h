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

const unsigned int BUFFER_SIZE = 512;

const std::string VERSION = "s0.0";
