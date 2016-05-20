//
// Created by wo on 5/18/16.
//

#ifndef RRR_SURFACES_H
#define RRR_SURFACES_H

#include "../math/vecp.h"
#include "../math/vec.h"
#include "ray_pk4.h"
#include "../portable.h"

struct aabb
{
  fp1v xmin, xmax, ymin, ymax, zmin, zmax;
  fp4v xmin4, xmax4, ymin4, ymax4, zmin4, zmax4;
};

typedef struct aabb r_aabb;

#define surface(w) struct surf_ ## w\
{\
  char SURF_TYPE;\
  r_aabb aabb;
#define end(w) };\
typedef struct surf_ ## w r_surf_ ## w;

#define def_p(x) p31v x; p34v x ## s;
#define def_v(x) v31v x; v34v x ## s;
#define def_s(x) fp1v x; fp4v x ## s;

surface()
end()
#define SURF_TYPE_ 0

surface(triangle)
  def_p(p)
  def_p(q)
  def_p(r)
  def_v(n)
end(triangle)
#define SURF_TYPE_TRIANGLE 1

surface(bezier)
end(bezier)
#define SURF_TYPE_BEZIER 2

surface(sphere)
  def_p(o)
  def_s(r)
end(sphere)
#define SURF_TYPE_SPHERE 3

#undef surface
#undef end
#undef def_p
#undef def_s
#undef def_v

struct isect_pk4_t
{
  r_surf_* surf;
  fp4v ts;
  v34v ns;
};
typedef struct isect_pk4_t isc4v;

void
make_surf_tri(r_surf_triangle* tri, p31v* p, p31v* q, p31v* r);

i4v
hit(r_surf_* surf, rp4* ray, isc4v* isc, fp1v t0, fp1v t1);

i4v
hit_aabb(r_aabb* aabb, rp4* ray, fp1v t0, fp1v t1);

#endif //RRR_SURFACES_H
