#pragma once
#include <iostream>

enum entity_type {
      NIPPER,
      MOONBOY,
      TESLA,
      TRAP,
      WORKER,
      BUILDER,
      FLYING,
      HIPSTER,
      LASER,
      N_ENTITIES
};

struct visual_entity {
      int id;
      entity_type et;
      int x, y, angle;
      int health, stamina;

      friend std::ostream &operator<<(std::ostream &os, visual_entity &ve);

      void add_to_buff(uint8_t* buff, int& a);
};
