//
// Created by wo on 5/21/16.
//

#include "vcommon.h"
#include "stdio.h"

v31v
vp4_tov1(v34v* v, char type)
{
  v31v ret;

  switch (type) {
  case 'a':
#define AVG(xmm) ((ATTR_FV4(xmm, 0) + ATTR_FV4(xmm, 1) + ATTR_FV4(xmm, 2) + ATTR_FV4(xmm, 3)) / 4)
    ret.x = AVG(v->xs);
    ret.y = AVG(v->ys);
    ret.z = AVG(v->zs);
#undef AVG
    break;
  case 's':
    ret.x = ATTR_VP4(v, x, 0);
    ret.y = ATTR_VP4(v, y, 0);
    ret.z = ATTR_VP4(v, z, 0);
    break;
  default:
    ret.x = ATTR_VP4(v, x, 0);
    ret.y = ATTR_VP4(v, y, 0);
    ret.z = ATTR_VP4(v, z, 0);
  }

  return ret;
}

v34v
v1_tovp4(v31v* v)
{
  v34v ret;
  ret.xs = SET1_fp4v(v->x);
  ret.ys = SET1_fp4v(v->y);
  ret.zs = SET1_fp4v(v->z);

  return ret;
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
  p_fp4v(&r->xs); printf(";");
  p_fp4v(&r->ys); printf(";");
  p_fp4v(&r->zs); printf(";");
}