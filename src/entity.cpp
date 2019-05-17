/*
  contains code for entities which include all non-ui, non-background things
  such as ants and structures
*/
#include "common.h"
#include "entity.h"
#include "network.h"

std::ostream &operator<<(std::ostream &os, visual_entity &ve) {
      os << "id: " << ve.id << std::endl;
      os << "et: " << ve.et << std::endl;
      os << "(x, y): (" << ve.x << ", " << ve.y << ")" << std::endl;
      os << "angle: " << ve.angle << std::endl;
      os << "health: " << ve.health << std::endl;
      os << "stamina: " << ve.stamina << std::endl << std::endl;

      return os;
}

void visual_entity::add_to_buff(uint8_t *buff, int &a) {
      uint32_to_uint8_t(id, buff, a);
      uint32_to_uint8_t(et, buff, a);
      uint32_to_uint8_t(x, buff, a);
      uint32_to_uint8_t(y, buff, a);
      uint32_to_uint8_t(angle, buff, a);
      uint32_to_uint8_t(health, buff, a);
      uint32_to_uint8_t(stamina, buff, a);
}
