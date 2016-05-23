//
// Created by wo on 5/21/16.
//

#include "vcommon.h"
#include "stdio.h"

void
vp4_tov1(v31v* r, v34v* v, char type)
{
  switch (type) {
  case 'a':
#define AVG(xmm) ((ATTR_FV4(xmm, 0) + ATTR_FV4(xmm, 1) + ATTR_FV4(xmm, 2) + ATTR_FV4(xmm, 3)) / 4)
    r->x = AVG(v->xs);
    r->y = AVG(v->ys);
    r->z = AVG(v->zs);
#undef AVG
    break;
  case 's':
    r->x = ATTR_VP4(v, x, 0);
    r->y = ATTR_VP4(v, y, 0);
    r->z = ATTR_VP4(v, z, 0);
    break;
  default:
    r->x = ATTR_VP4(v, x, 0);
    r->y = ATTR_VP4(v, y, 0);
    r->z = ATTR_VP4(v, z, 0);
  }
}

void
v1_tovp4(v34v* r, v31v* v)
{
  vp4_load1(r, v->x, v->y, v->z);
}

void
p_fp4v(fp4v* r)
{
  printf("(" FPFMT "," FPFMT "," FPFMT "," FPFMT ")",
         ATTR_FV4(*r, 3),
         ATTR_FV4(*r, 2),
         ATTR_FV4(*r, 1),
         ATTR_FV4(*r, 0));
}

void
p_v34v(v34v* r)
{
  p_fp4v(&r->xs); printf(";\n");
  p_fp4v(&r->ys); printf(";\n");
  p_fp4v(&r->zs); printf(";");
}