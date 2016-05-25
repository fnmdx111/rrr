//
// Created by wo on 5/18/16.
//

#ifndef RRR_SCENE_H
#define RRR_SCENE_H

#include "surfaces.h"
#include "lights.h"
#include "../generic/aarr.h"

struct rgba
{
  fp1v r, g, b, a;
};
typedef struct rgba r_rgba;

void
make_rgba(r_rgba* rgba, fp1v r, fp1v g, fp1v b, fp1v a);

struct camera
{
  p34v pos, dir, u, v, w;
  fp4v d, img_w, img_h;
  fp4v pxl_w, pxl_h;
  fp4v r, l, t, b;
  fp4v lens;
  fp4v aperture_size;
};
typedef struct camera r_camera;

void
make_camera(r_camera* cam,
            p31v pos, v31v dir,
            fp1v d,
            fp1v img_w, fp1v img_h,
            int pxl_w, int pxl_h,
            fp1v lens, fp1v aperture_size);

struct r_mtr_phong_bling
{
  c31v diff, spec, ispec, refr;
  int spec_r, glossy;
  fp1v nidx;
};

struct r_mtr_custom_shading
{
  SHADING_FUNC shader;
};

enum r_mtr_type
{
  MTR_PHONG_BLING,
  MTR_CUSTOM
};
struct material
{
  enum r_mtr_type type;
  union {
    struct r_mtr_phong_bling phong_bling;
    struct r_mtr_custom_shading custom;
  };
};
typedef struct material r_mtr;

struct scene
{
  struct r_aarr* surfs;
  struct r_aarr* lights;
  struct r_aarr* materials;

  r_camera* cam;
};

#define push_surf(scene, surfp) aarr_push((scene)->surfs, r_surf, &surfp)

typedef struct scene r_scene;

void
make_scene(r_scene* scene);

void
free_scene(r_scene* scene);

void
init_scene_by_config(r_scene* scene, char* fn);

void
append_wavefront_object(struct r_aarr *buf, const char *fn);

void
destroy_scene(r_scene* scene);

void
render(r_scene* scene);

#endif //RRR_SCENE_H
