//
// Created by wo on 5/18/16.
//

#ifndef RRR_LIGHTS_H
#define RRR_LIGHTS_H

#include "../math/vec.h"

struct r_ambient_light
{
  v31v color;
};

struct r_point_light
{
  p31v pos;
  v31v color;
};

struct r_direction_light
{
  v31v dir;
  v31v color;
};

struct r_area_light
{
};

enum r_light_type
{
  AMBIENT, POINT, DIRECTION, AREA
};
struct light
{
  enum r_light_type type;
  union {
    struct r_ambient_light ambient;
    struct r_point_light point;
    struct r_direction_light direction;
    struct r_area_light area;
  };
};

typedef struct light r_light;

#endif //RRR_LIGHTS_H
