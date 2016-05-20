//
// Created by wo on 5/18/16.
//

#include "surfaces.h"
#include "../portable.h"

i4v
hit_aabb(r_aabb* aabb, rp4* ray, fp1v t0, fp1v t1)
{
  fp4v mask;
  i4v ret = _mm_set1_epi32(1);

  fp4v tnear = SET1_fp4v(t0);
  fp4v tfar = SET1_fp4v(t1);
  fp4v which_normal = ZERO_fp4v();
#define test(axis) do {\
  fp4v rep_d = RCP_fp4v(ray->d. axis ## s);\
  fp4v tmin = MUL_fp4v(rep_d, SUB_fp4v(aabb-> axis ## min4, ray->d. axis ## s));\
  fp4v tmax = MUL_fp4v(rep_d, SUB_fp4v(aabb-> axis ## max4, ray->d. axis ## s));\
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

  return ret;
}

i4v
hit_tri(r_surf_triangle* surf, rp4* ray, isc4v* isc, fp1v t0, fp1v t1)
{
#define def(x, z) fp4v x = (z)

  def(a, SUB_fp4v(surf->ps.xs, surf->qs.xs));
  def(b, SUB_fp4v(surf->ps.ys, surf->qs.ys));
  def(c, SUB_fp4v(surf->ps.zs, surf->qs.zs));
  def(d, SUB_fp4v(surf->ps.xs, surf->rs.xs));
  def(e, SUB_fp4v(surf->ps.ys, surf->rs.ys));
  def(f, SUB_fp4v(surf->ps.zs, surf->rs.zs));
  def(g, ray->d.xs);
  def(h, ray->d.ys);
  def(i, ray->d.zs);
  def(j, SUB_fp4v(surf->ps.xs, ray->e.xs));
  def(k, SUB_fp4v(surf->ps.ys, ray->e.ys));
  def(l, SUB_fp4v(surf->ps.zs, ray->e.zs));

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

  isc->surf = (r_surf_*) surf;
  isc->ts = AND_fp4v((__m128) ret, t);
  isc->ns.xs = AND_fp4v((__m128) ret, surf->ns.xs);
  isc->ns.ys = AND_fp4v((__m128) ret, surf->ns.ys);
  isc->ns.zs = AND_fp4v((__m128) ret, surf->ns.zs);

  return ret;
}

i4v
hit(r_surf_* surf, rp4* ray, isc4v* isc, fp1v t0, fp1v t1)
{
  if (ALL_ZERO_i4v(hit_aabb(&(surf->aabb), ray, t0, t1))) {
    return _mm_set1_epi32(0);
  }

  switch (surf->SURF_TYPE) {
  case SURF_TYPE_TRIANGLE:
    return hit_tri((r_surf_triangle*) surf, ray, isc, t0, t1);
  default:
    exit(0);
  }
}

void
make_surf_tri(r_surf_triangle* tri, p31v* p, p31v* q, p31v* r)
{

}
