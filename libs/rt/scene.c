//
// Created by wo on 5/18/16.
//

#include <stdio.h>
#include "scene.h"
#include "../math/vcommon.h"

void
make_scene(r_scene* scene)
{
  make_aarr(scene->surfs, sizeof(r_surf), 0);
  /**
   * r_surf have to be stored in consecutive memory because of
   * cache-related considerations.
   */
  make_aarr(scene->lights, sizeof(r_light), 0);
  make_aarr(scene->materials, sizeof(r_mtr), 0);
}

void
free_scene(r_scene* scene)
{
  /**
   * r_scene does not own aarr buffers, so it does not handle the
   * destruction of such data structure.
   */
}

void
append_wavefront_object(struct r_aarr *buf, const char *fn)
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
  r_surf this_tri;
  int p, q, r;
  char __;

  f = fopen(fn, "r");
  while (1) {
    ssize_t x = getline(&line, &line_size, f);

    if (x == -1) {
      break;
    }

    switch (line[0]) {
    case 'v':
      sscanf(line, "%c " FPFMT " " FPFMT " " FPFMT "\n",
             &__, &this_vtx.x, &this_vtx.y, &this_vtx.z);
      vtxs[vcnt] = this_vtx;
      ++vcnt;
      break;

    case 'f':
      sscanf(line, "%c %d %d %d\n", &__, &p, &q, &r);
      --p; --q; --r;

      this_tri.type = SURF_TRIANGLE;
      make_surf_tri(&(this_tri.triangle),
                    &vtxs[p], &vtxs[q], &vtxs[r]);

      aarr_push(buf, r_surf, &this_tri);

    default:
      continue;
    }
  }
  fclose(f);
  if (line != NULL) {
    free(line);
  }

  free(vtxs);
}

void
make_rgba(r_rgba* rgba, fp1v r, fp1v g, fp1v b, fp1v a)
{
  rgba->r = r;
  rgba->g = g;
  rgba->b = b;
  rgba->a = a;
}

void
make_camera(r_camera* cam,
            p31v pos, v31v dir,
            fp1v d, fp1v img_w, fp1v img_h,
            int pxl_w, int pxl_h,
            fp1v lens, fp1v aperture_size)
{
  v1_tovp4(&cam->pos, &pos);
  v1_tovp4(&cam->dir, &dir);

  v34v up;
  vp4_load1(&up, 0.f, 1.f, 0.f);

  vp4_cross(&cam->u, &cam->dir, &up);
  vp4_cross(&cam->v, &cam->u, &cam->dir);
  vp4_neg(&cam->w, &cam->dir);

  vp4_normalize(&cam->u, &cam->u);
  vp4_normalize(&cam->v, &cam->v);
  vp4_normalize(&cam->w, &cam->w);
  vp4_normalize(&cam->dir, &cam->dir);

  cam->d = SET1_fp4v(d);

  cam->img_w = SET1_fp4v(img_w);
  cam->img_h = SET1_fp4v(img_h);

  cam->pxl_w = SET1_fp4v(pxl_w);
  cam->pxl_h = SET1_fp4v(pxl_h);

  cam->r = DIV_fp4v(cam->img_w, SET1_fp4v(2.0));
  cam->l = NEG_fp4v(cam->r);
  cam->t = DIV_fp4v(cam->img_h, SET1_fp4v(2.0));
  cam->b = NEG_fp4v(cam->t);

  cam->lens = SET1_fp4v(lens);
  cam->aperture_size = SET1_fp4v(aperture_size);
}

void
render(r_scene* scene)
{
  printf("Rendering...\n");
}