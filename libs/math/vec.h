//
// Created by wo on 5/18/16.
//

#ifndef RRR_VEC_H
#define RRR_VEC_H

#include "../portable.h"

struct vec3
{
  fp1v x, y, z;
};

typedef struct vec3 v31v;
typedef struct vec3 p31v;
typedef struct vec3 c31v;

void
v1_cross(v31v* r, v31v* p, v31v* q);

void
v1_sub(v31v* r, v31v* p, v31v* q);

#endif //RRR_VEC_H
