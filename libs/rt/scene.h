//
// Created by wo on 5/18/16.
//

#ifndef RRR_SCENE_H
#define RRR_SCENE_H

#include "surfaces.h"
#include "lights.h"
#include "../generic/aarr.h"

struct camera
{

};
typedef struct camera r_camera;

struct material
{

};
typedef struct material r_mtr;

struct scene
{
  struct r_aarr surfs;
  struct r_aarr lights;
  struct r_aarr materials;

  r_camera cam;
};

#define push_surf(scene, surfp) aarr_push(&((scene)->surfs), r_surf_*, &((r_surf_*) (surfp)))

typedef struct scene r_scene;

void
make_scene(r_scene* scene);

void
free_scene(r_scene* scene);

void
init_scene_by_config(r_scene* scene, char* fn);

void
load_wavefront_object(r_scene* scene, char* fn);

void
destroy_scene(r_scene* scene);

#endif //RRR_SCENE_H
