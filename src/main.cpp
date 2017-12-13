#include "common.h"
#include "network.h"
#include "entity.h"
#include "maps.h"
#include <fstream>

int main (int argc, char* argv[])
{
	map map;
	std::ifstream f(argv[1]);
	map.read_map(f);
	return 0;
}
