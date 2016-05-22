//
// Created by wo on 5/17/16.
//

#ifndef RRR_FUNT_H
#define RRR_FUNT_H

#include <xmmintrin.h>
#include <immintrin.h>
#include "../portable.h"

struct vec_pk4_t
{
  fp4v xs, ys, zs;
};

#define MAKE_VP4(x) v34v* x = (v34v*) malloc(sizeof(v34v))
#define FREE_VP4(x) free(x)
#define ATTR_FV4(x, k) (EXTR_fp4v(x, (k)))
#define ATTR_VP4(x, d, k) (ATTR_FV4(x->d ## s, k))

typedef struct vec_pk4_t v34v;
typedef struct vec_pk4_t p34v;

void
vp4_load1(v34v* v, fp1v x, fp1v y, fp1v z);

void
vp4_load2(v34v* v,
          fp1v x1, fp1v y1, fp1v z1,
          fp1v x2, fp1v y2, fp1v z2);

void
vp4_load4(v34v* v,
          fp1v x1, fp1v y1, fp1v z1,
          fp1v x2, fp1v y2, fp1v z2,
          fp1v x3, fp1v y3, fp1v z3,
          fp1v x4, fp1v y4, fp1v z4);

fp4v
vp4_dot(v34v *p, v34v *q);

void
vp4_cross(v34v *r, v34v *p, v34v *q);
#endif //RRR_FUNT_H

void
vp4_add(v34v *r, v34v *p, v34v *q);

void
vp4_sub(v34v* r, v34v* p, v34v* q);

void
vp4_neg(v34v* r, v34v* p);

void
vp4_normalize(v34v* r, v34v* p);

fp4v
vp4_norm(v34v* r);