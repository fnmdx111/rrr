//
// Created by wo on 5/18/16.
//

#include "surfaces.h"
#include "../portable.h"
#include "../math/common.h"

i4v
hit_aabb(r_aabb* aabb, rp4* ray, fp1v t0, fp1v t1)
{
  fp4v mask;
  i4v ret = _mm_set1_epi32(1);

  fp4v tnear = SET1_fp4v(t0);
  fp4v tfar = SET1_fp4v(t1);
  fp4v which_normal = ZERO_fp4v();
#define test(axis) do {\
  fp4v axis ## min4 = SET1_fp4v(aabb->axis ## min);\
  fp4v axis ## max4 = SET1_fp4v(aabb->axis ## max);\
  fp4v rep_d = RCP_fp4v(ray->d. axis ## s);\
  fp4v tmin = MUL_fp4v(rep_d, SUB_fp4v(axis ## min4, ray->d. axis ## s));\
  fp4v tmax = MUL_fp4v(rep_d, SUB_fp4v(axis ## max4, ray->d. axis ## s));\
\
  fp4v zero = ZERO_fp4v();\
  mask = LT_fp4v(rep_d, zero);\
  fp4v n_tmin = SELECT2_fp4v(mask, tmin, tmax);\
  fp4v n_tmax = SELECT2_fp4v(mask, tmax, tmin);\
\
  mask = LT_fp4v(tnear, n_tmin);\
  tnear = SELECT2_fp4v(mask, tnear, n_tmin);\
  which_normal = SELECT2_fp4v(mask, which_normal, SET1_fp4v(1.0));\
  tfar = MIN_fp4v(tfar, n_tmax);\
\
  ret = FILTER_BY_i4v(GT_fp4v(tnear, tfar), ret);\
  if (_mm_testz_si128(ret, ret) == 0) {\
    return ret;\
  }\
} while (0)

  test(x);
  test(y);
  test(z);
#undef test

  return ret;
}

i4v
hit_tri(r_surf_triangle* surf, rp4* ray, isc4v* isc, fp1v t0, fp1v t1)
{
#define def(x, z) fp4v x = (z)

  v34v ps, qs, rs, ns;
  vp4_load1(&ps, surf->p.x, surf->p.y, surf->p.z);
  vp4_load1(&qs, surf->q.x, surf->q.y, surf->q.z);
  vp4_load1(&rs, surf->r.x, surf->r.y, surf->r.z);
  vp4_load1(&ns, surf->n.x, surf->n.y, surf->n.z);

#define a(p, x) (p ## s.x ## s)
#define ap(x) a(p, x)
#define aq(x) a(q, x)
#define ar(x) a(r, x)
#define an(x) a(n, x)

  def(a, SUB_fp4v(ap(x), aq(x)));
  def(b, SUB_fp4v(ap(y), aq(y)));
  def(c, SUB_fp4v(ap(z), aq(z)));
  def(d, SUB_fp4v(ap(x), ar(x)));
  def(e, SUB_fp4v(ap(y), ar(y)));
  def(f, SUB_fp4v(ap(z), ar(z)));
  def(g, ray->d.xs);
  def(h, ray->d.ys);
  def(i, ray->d.zs);
  def(j, SUB_fp4v(ap(x), ray->e.xs));
  def(k, SUB_fp4v(ap(y), ray->e.ys));
  def(l, SUB_fp4v(ap(z), ray->e.zs));

  def(eimhf, SUB_fp4v(MUL_fp4v(e, i), MUL_fp4v(h, f)));
  def(gfmdi, SUB_fp4v(MUL_fp4v(g, f), MUL_fp4v(d, i)));
  def(dhmeg, SUB_fp4v(MUL_fp4v(d, h), MUL_fp4v(e, g)));
  def(akmjb, SUB_fp4v(MUL_fp4v(a, k), MUL_fp4v(j, b)));
  def(jcmal, SUB_fp4v(MUL_fp4v(j, c), MUL_fp4v(a, l)));
  def(blmkc, SUB_fp4v(MUL_fp4v(b, l), MUL_fp4v(k, c)));

  def(M, RCP_fp4v(ADD_fp4v(ADD_fp4v(MUL_fp4v(a, eimhf),
                                    MUL_fp4v(b, gfmdi)),
                           MUL_fp4v(c, dhmeg))));
  def(t, NEG_fp4v(MUL_fp4v(M,
                           ADD_fp4v(MUL_fp4v(f, akmjb),
                                    ADD_fp4v(MUL_fp4v(e, jcmal),
                                             MUL_fp4v(d, blmkc))))));

  i4v ret = _mm_set1_epi32(UINT32_MAX);
  ret = FILTER_BY_i4v(LT_fp4v(t, SET1_fp4v(t0)), ret);
  ret = FILTER_BY_i4v(GT_fp4v(t, SET1_fp4v(t1)), ret);
  if (ALL_ZERO_i4v(ret)) {
    return ret;
  }

  def(C, MUL_fp4v(M, ADD_fp4v(ADD_fp4v(MUL_fp4v(i, akmjb),
                                       MUL_fp4v(h, jcmal)),
                              MUL_fp4v(g, blmkc))));
  ret = FILTER_BY_i4v(LT_fp4v(C, SET1_fp4v(0.0)), ret);
  ret = FILTER_BY_i4v(GT_fp4v(C, SET1_fp4v(1.0)), ret);
  if (ALL_ZERO_i4v(ret)) {
    return ret;
  }

  def(B, MUL_fp4v(M, ADD_fp4v(ADD_fp4v(MUL_fp4v(j, eimhf),
                                       MUL_fp4v(k, gfmdi)),
                              MUL_fp4v(l, dhmeg))));
  ret = FILTER_BY_i4v(LT_fp4v(B, SET1_fp4v(0.0)), ret);
  ret = FILTER_BY_i4v(GT_fp4v(B, SUB_fp4v(SET1_fp4v(1.0), C)), ret);
  if (ALL_ZERO_i4v(ret)) {
    return ret;
  }

  isc->surf = (r_surf*) surf;
  isc->ts = AND_fp4v((__m128) ret, t);
  isc->ns.xs = AND_fp4v((__m128) ret, an(x));
  isc->ns.ys = AND_fp4v((__m128) ret, an(y));
  isc->ns.zs = AND_fp4v((__m128) ret, an(z));

#undef a
#undef ap
#undef aq
#undef ar
#undef an
#undef def

  return ret;
}

i4v
hit(r_surf* surf, rp4* ray, isc4v* isc, fp1v t0, fp1v t1)
{
  if (ALL_ZERO_i4v(hit_aabb(&(surf->aabb), ray, t0, t1))) {
    return _mm_set1_epi32(0);
  }

  switch (surf->type) {
  case SURF_TRIANGLE:
    return hit_tri((r_surf_triangle*) surf, ray, isc, t0, t1);
  default:
    exit(0);
  }
}

void
make_surf_tri(r_surf* surf, p31v* p, p31v* q, p31v* r)
{
  surf->type = SURF_TRIANGLE;

  r_aabb* aabb = &surf->aabb;
#define _set_aabb(axis)\
  aabb->axis ## min = min3(p->axis, q->axis, r->axis);\
  aabb->axis ## max = max3(p->axis, q->axis, r->axis)

  _set_aabb(x);
  _set_aabb(y);
  _set_aabb(z);
#undef _set_aabb

  r_surf_triangle* tri = &surf->triangle;
  tri->p = *p;
  tri->q = *q;
  tri->r = *r;

  v31v pq, pr;
  v1_sub(&pq, q, p);
  v1_sub(&pr, r, p);

  v1_cross(&tri->n, &pq, &pr);
}

void
make_surf_sph(r_surf* sph, p31v* o, fp1v* r)
{
  sph->type = SURF_SPHERE;

  fp1v rr = *r;
  r_aabb* aabb = &sph->aabb;

#define _set_aabb(axis)\
  aabb->axis ## min = o->axis - rr;\
  aabb->axis ## max = o->axis + rr

  _set_aabb(x);
  _set_aabb(y);
  _set_aabb(z);
#undef _set_aabb

  sph->sphere.o = *o;
  sph->sphere.r = rr;
}