/*
  contains code for entities which include all non-ui, non-background things
  such as ants and structures
*/
#include "entity.h"

std::ostream &operator<<(std::ostream &os, visual_entity &ve) {
      os << "id: " << ve.id << std::endl;
      os << "et: " << ve.et << std::endl;
      os << "(x, y): (" << ve.x << ", " << ve.y << ")" << std::endl;
      os << "angle: " << ve.angle << std::endl;
      os << "health: " << ve.health << std::endl;
      os << "stamina: " << ve.stamina << std::endl << std::endl;

      return os;
}
