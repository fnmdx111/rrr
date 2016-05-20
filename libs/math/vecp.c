//
// Created by wo on 5/17/16.
//

#include "vecp.h"

inline void
vp4_load1(v34v* v, fp1v x, fp1v y, fp1v z)
{
  v->xs = SET_fp4v(x, x, x, x);
  v->ys = SET_fp4v(y, y, y, y);
  v->zs = SET_fp4v(z, z, z, z);
}

inline void
vp4_load2(v34v* v,
          fp1v x1, fp1v y1, fp1v z1,
          fp1v x2, fp1v y2, fp1v z2)
{
  v->xs = SET_fp4v(x2, x2, x1, x1);
  v->ys = SET_fp4v(y2, y2, y1, y1);
  v->zs = SET_fp4v(z2, z2, z1, z1);
}

inline void
vp4_load4(v34v* v,
          fp1v x1, fp1v y1, fp1v z1,
          fp1v x2, fp1v y2, fp1v z2,
          fp1v x3, fp1v y3, fp1v z3,
          fp1v x4, fp1v y4, fp1v z4)
{
  v->xs = SET_fp4v(x4, x3, x2, x1);
  v->ys = SET_fp4v(y4, y3, y2, y1);
  v->zs = SET_fp4v(z4, z3, z2, z1);
}

inline fp4v
vp4_dot(v34v *p, v34v *q)
{
  fp4v x = MUL_fp4v(p->xs, q->xs);
  fp4v y = MUL_fp4v(p->ys, q->ys);
  fp4v z = MUL_fp4v(p->zs, q->zs);

  fp4v r = ADD_fp4v(x, y);
  r = _mm_add_ps(r, z);

  return r;
}

inline void
vp4_cross(v34v *r, v34v *p, v34v *q)
{
  fp4v u2v3 = MUL_fp4v(p->ys, q->zs);
  fp4v u3v2 = MUL_fp4v(p->zs, q->ys);
  r->xs = SUB_fp4v(u2v3, u3v2);

  fp4v u3v1 = MUL_fp4v(p->zs, q->xs);
  fp4v u1v3 = MUL_fp4v(p->xs, q->zs);
  r->ys = SUB_fp4v(u3v1, u1v3);

  fp4v u1v2 = MUL_fp4v(p->xs, q->ys);
  fp4v u2v1 = MUL_fp4v(p->ys, q->xs);
  r->zs = SUB_fp4v(u1v2, u2v1);
}

inline void
vp4_add(v34v *r, v34v *p, v34v *q)
{
  r->xs = p->xs + q->xs;
  r->ys = p->ys + q->ys;
  r->zs = p->zs + q->zs;
}

inline void
vp4_normalize(v34v* r)
{
  fp4v xsq = MUL_fp4v(r->xs, r->xs);
  fp4v ysq = MUL_fp4v(r->ys, r->ys);
  fp4v zsq = MUL_fp4v(r->zs, r->zs);
  fp4v xyzsq = ADD_fp4v(xsq, ysq);
  xyzsq = ADD_fp4v(xyzsq, zsq);

  fp4v rxyzsq = RSQRT_fp4v(xyzsq);
  r->xs = MUL_fp4v(r->xs, rxyzsq);
  r->ys = MUL_fp4v(r->ys, rxyzsq);
  r->zs = MUL_fp4v(r->zs, rxyzsq);
}
