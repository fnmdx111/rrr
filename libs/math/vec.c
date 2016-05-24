//
// Created by wo on 5/18/16.
//

#include "vec.h"

inline void
v1_cross(v31v* r, v31v* p, v31v* q)
{
  fp1v u2v3 = p->y * q->z;
  fp1v u3v2 = p->z * q->y;
  r->x = u2v3 - u3v2;

  fp1v u3v1 = p->z * q->x;
  fp1v u1v3 = p->x * q->z;
  r->y = u3v1 - u1v3;

  fp1v u1v2 = p->x * q->y;
  fp1v u2v1 = p->y * q->x;
  r->z = u1v2 - u2v1;
}

inline void
v1_sub(v31v* r, v31v* p, v31v* q)
{
  r->x = p->x - q->x;
  r->y = p->y - q->y;
  r->z = p->z - q->z;
}
