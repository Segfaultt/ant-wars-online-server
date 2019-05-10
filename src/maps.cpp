#include "common.h"

struct coord {
      int x, y;
      friend std::ostream &operator<<(std::ostream &os, coord coord);
      void read_coord(std::istream &is);
};

struct map {
      std::vector<std::vector<coord>> coastlines;

      void add_point(int coastline, int x, int y);
      int add_coastline(); // returns new coastline index
      void read_map(std::istream &is);
      friend std::ostream &operator<<(std::ostream &os, map map);
};

std::ostream &operator<<(std::ostream &os, coord coord) {
      os << coord.x << ',' << coord.y << std::endl;
}

void map::add_point(int coastline, int x, int y) {
      coord point = {x, y};

      coastlines[coastline].push_back(point);
}

int map::add_coastline() {
      std::vector<coord> temp_coastline = {};
      coastlines.push_back(temp_coastline);

      return coastlines.size() - 1;
}

std::ostream &operator<<(std::ostream &os, map map) {
      for (std::vector<coord> coastline : map.coastlines) {
            os << '/' << std::endl;
            for (coord point : coastline)
                  os << point;
      }

      return os;
}

void map::read_map(std::istream &is) {
      std::string buffer;
      int index = -1;
      while (is.good()) {
            if (is.peek() == '/') {
                  index = add_coastline();
                  is.ignore();
                  if (index > 2)
                        std::cout << "reading" << index + 1 << "th coastline"
                                  << std::endl;
                  else if (index == 0)
                        std::cout << "reading first coastline" << std::endl;
                  else if (index == 1)
                        std::cout << "reading second coastline" << std::endl;
                  else if (index == 2)
                        std::cout << "reading third coastline" << std::endl;
            }
            add_point(index, 0, 0);
            coastlines[index][coastlines[index].size() - 1].read_coord(is);
      }
      coastlines[index].pop_back();
      ;
}

void coord::read_coord(std::istream &is) {
      char transfer[256];
      is.getline(transfer, 256, ',');
      x = atoi(transfer);
      is.getline(transfer, 256);
      y = atoi(transfer);
}
