//
// Created by wo on 5/18/16.
//

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include "scene.h"

void
make_scene(r_scene* scene)
{
  make_aarr(&scene->surfs, sizeof(r_surf_*), 0);
  make_aarr(&scene->lights, sizeof(r_light*), 0);
  make_aarr(&scene->materials, sizeof(r_mtr*), 0);
}

void
free_scene(r_scene* scene)
{
  aarr_for(&scene->surfs, r_surf_*, it)
    free(it);
  aarr_end

  aarr_for(&scene->lights, r_light*, it)
    free(it);
  aarr_end

  aarr_for(&scene->materials, r_mtr*, it)
    free(it);
  aarr_end
}

void
init_scene_by_config(r_scene* scene, char* fn)
{
  lua_State* L = luaL_newstate();
  luaL_openlibs(L);

  lua_close(L);
}

void
load_wavefront_object(r_scene* scene, char* fn)
{
  FILE* f = fopen(fn, "r");

  size_t vcnt = 0;
  size_t tcnt = 0;
  size_t line_size = 0;
  char* line = NULL;
  while (1) {
    ssize_t x = getline(&line, &line_size, f);

    if (x == -1) {
      break;
    }

    switch (line[0]) {
    case 'v':
      ++vcnt;
      break;
    case 'f':
      ++tcnt;
      break;
    default:
      continue;
    }
  }
  fclose(f);
  if (line) {
    free(line);
  }

  p31v* vtxs = (p31v*) calloc(vcnt, sizeof(p31v));

  vcnt = 0;
  line_size = 0;
  line = NULL;

  p31v this_vtx;
  r_surf_triangle* this_tri = 0;
  size_t p, q, r;

  f = fopen(fn, "r");
  while (1) {
    ssize_t x = getline(&line, &line_size, f);

    if (x == -1) {
      break;
    }

    switch (line[0]) {
    case 'v':
      sscanf(line, FPFMT " " FPFMT " " FPFMT,
             &this_vtx.x, &this_vtx.y, &this_vtx.z);
      vtxs[vcnt] = this_vtx;
      ++vcnt;
      break;

    case 'f':
      sscanf(line, "%ld %ld %ld", &p, &q, &r);
      --p; --q; --r;

      this_tri = (r_surf_triangle*) malloc(sizeof(r_surf_triangle));
      make_surf_tri(this_tri, &vtxs[p], &vtxs[q], &vtxs[r]);

      push_surf(scene, this_tri);

    default:
      continue;
    }
  }
}