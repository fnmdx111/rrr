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
};

typedef struct aabb r_aabb;

#define surface(w) struct surf_ ## w\
{\

#define end(w) };\
typedef struct surf_ ## w r_surf_ ## w;

#define def_p(x) p31v x;
#define def_v(x) v31v x;
#define def_s(x) fp1v x;

surface(triangle)
  def_p(p)
  def_p(q)
  def_p(r)
  def_v(n)
end(triangle)

surface(bezier)
end(bezier)

surface(sphere)
  def_p(o)
  def_s(r)
end(sphere)

#undef surface
#undef end
#undef def_p
#undef def_s
#undef def_v

enum r_surf_type
{
  SURF_TRIANGLE,
  SURF_BEZIER,
  SURF_SPHERE
};

struct r_surf
{
  enum r_surf_type type;
  r_aabb aabb;
  union {
    r_surf_triangle triangle;
    r_surf_bezier bezier;
    r_surf_sphere sphere;
  };
};
typedef struct r_surf r_surf;

struct isect_pk4_t
{
  fp4v ts;
  v34v ns;
  struct r_surf* surf;
};
typedef struct isect_pk4_t isc4v;

void
make_surf_tri(r_surf_triangle* tri, p31v* p, p31v* q, p31v* r);

i4v
hit(r_surf* surf, rp4* ray, isc4v* isc, fp1v t0, fp1v t1);

i4v
hit_aabb(r_aabb* aabb, rp4* ray, fp1v t0, fp1v t1);

#endif //RRR_SURFACES_H
