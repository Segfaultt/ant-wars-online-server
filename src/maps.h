#include "common.h"

struct coord {
	int x, y;
	void read_coord(std::istream& is);
	friend std::ostream& operator<<(std::ostream& os, coord coord);
};
 
struct map {
	std::vector<std::vector<coord>> coastlines;
	
	void add_point(int coastline, int x, int y);
	int add_coastline();// returns new coastline index
	void read_map(std::istream& is);
	friend std::ostream& operator<<(std::ostream& os, map map);
};
