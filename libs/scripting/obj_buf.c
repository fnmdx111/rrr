//
// Created by wo on 5/20/16.
//

#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
#include "obj_buf.h"
#include "../rt/surfaces.h"
#include "../rt/lights.h"
#include "../rt/scene.h"
#include "scommon.h.h"

int
close_buf(lua_State* L)
{
  struct rl_g_buf* g = lua_touserdata(L, 1);
  free_aarr(g->hnd);

  return 0;
}

int
__gc_buf(lua_State* L)
{
  struct rl_g_buf* g = lua_touserdata(L, 1);
  (void) g;

  return 0;
}

int
size_buf(lua_State* L)
{
  struct rl_g_buf* g = lua_touserdata(L, 1);
  lua_pushnumber(L, g->hnd->size);

  return 1;
}

const struct luaL_Reg rl_surf_buf_funcs[] = {
  {"append_wavefront_object", l_append_wavefront_object},
  {"add_triangle", l_add_triangle},
  {"add_sphere", l_add_sphere},
  {"close", close_buf},
  {"size", size_buf},
  {NULL, NULL}
};

const struct luaL_Reg rl_light_buf_funcs[] = {
  {"add_ambient_light", l_add_ambient_light},
  {"add_point_light", l_add_point_light},
  {"close", close_buf},
  {"size", size_buf},
  {NULL, NULL}
};

const struct luaL_Reg rl_mtr_buf_funcs[] = {
  {"add_phongbling", l_add_phongbling},
  {"close", close_buf},
  {"size", size_buf},
  {NULL, NULL}
};

int
__tostring_buf(lua_State* L)
{
  struct rl_g_buf* g = lua_touserdata(L, 1);
  lua_pushfstring(L, "rl_*_buf <%p> of size %d", g, g->hnd->size);

  return 1;
}

void
install_buf_metatables(lua_State* L)
{
  /**
   * RLSurfaceBuf initialization
   */
  luaL_newmetatable(L, RL_SURF_BUF_MT_KEY);

  /**
   * Meta methods:
   *   + __gc
   */
  lua_pushcfunction(L, __gc_buf);
  lua_setfield(L, -2, "__gc");
  lua_pushcfunction(L, __tostring_buf);
  lua_setfield(L, -2, "__tostring");

  /**
   * Class methods:
   *   + new
   */
  lua_pushcfunction(L, l_create_surf_buf);
  lua_setfield(L, -2, "new");

  /**
   * Instance methods:
   *   + append_wavefront_object
   */
  luaL_newlib(L, rl_surf_buf_funcs);
  lua_setfield(L, -2, "__index");
  lua_setglobal(L, "RLSurfaceBuf");

  /**
   * RLLightBuf initialization
   */
  luaL_newmetatable(L, RL_LIGHT_BUF_MT_KEY);

  lua_pushcfunction(L, __gc_buf);
  lua_setfield(L, -2, "__gc");
  lua_pushcfunction(L, __tostring_buf);
  lua_setfield(L, -2, "__tostring");

  lua_pushcfunction(L, l_create_light_buf);
  lua_setfield(L, -2, "new");

  luaL_newlib(L, rl_light_buf_funcs);
  lua_setfield(L, -2, "__index");
  lua_setglobal(L, "RLLightBuf");

  /**
   * RLMaterialBuf initialization
   */
  luaL_newmetatable(L, RL_MATERIAL_BUF_MT_KEY);

  lua_pushcfunction(L, __gc_buf);
  lua_setfield(L, -2, "__gc");
  lua_pushcfunction(L, __tostring_buf);
  lua_setfield(L, -2, "__tostring");

  lua_pushcfunction(L, l_create_material_buf);
  lua_setfield(L, -2, "new");

  luaL_newlib(L, rl_mtr_buf_funcs);
  lua_setfield(L, -2, "__index");
  lua_setglobal(L, "RLMaterialBuf");
}

int
luaopen_objbuf(lua_State* L)
{
  return 1;
}

int
l_create_surf_buf(lua_State* L)
{
  struct rl_surf_buf* surf = lua_newuserdata(L, sizeof(struct rl_surf_buf));
  surf->hnd = malloc(sizeof(struct r_aarr));

  make_aarr(surf->hnd, sizeof(r_surf), 0);

  luaL_setmetatable(L, RL_SURF_BUF_MT_KEY);
  return 1;
}

int
l_create_light_buf(lua_State* L)
{
  struct rl_light_buf* light = lua_newuserdata(L, sizeof(struct rl_light_buf));
  light->hnd = malloc(sizeof(struct r_aarr));

  make_aarr(light->hnd, sizeof(r_light), 0);

  luaL_setmetatable(L, RL_LIGHT_BUF_MT_KEY);
  return 1;
}

int
l_create_material_buf(lua_State* L)
{
  struct rl_material_buf* mtr = lua_newuserdata(L,
                                                sizeof(struct rl_material_buf));
  mtr->hnd = malloc(sizeof(struct r_aarr));

  make_aarr(mtr->hnd, sizeof(r_mtr), 0);

  luaL_setmetatable(L, RL_MATERIAL_BUF_MT_KEY);
  return 1;
}

int
l_append_wavefront_object(lua_State *L)
{
  struct rl_surf_buf* s = check_surf_buf(L, 1);
  const char* fn = lua_tostring(L, 2);
  append_wavefront_object(s->hnd, fn);

  create_wrapper(L, (struct rl_g_buf *) s, RL_SURF_BUF_MT_KEY);

  return 0;
}

int
l_add_triangle(lua_State* L)
{
  struct rl_surf_buf* s = check_surf_buf(L, 1);

  r_surf tri;
  v31v p, q, r;

  s_get_v31v(p, 2);
  s_get_v31v(q, 3);
  s_get_v31v(r, 4);

  make_surf_tri(&tri, &p, &q, &r);

  aarr_push(s->hnd, r_surf, &tri);

  create_wrapper(L, (struct rl_g_buf *) s, RL_SURF_BUF_MT_KEY);

  return 1;
}

int
l_add_sphere(lua_State* L)
{
  struct rl_surf_buf* s = check_surf_buf(L, 1);

  r_surf sph;

  p31v o;
  fp1v r;

  s_get_v31v(o, 2);
  r = s_get_fp1v(3);

  make_surf_sph(&sph, &o, &r);
  aarr_push(s->hnd, r_surf, &sph);

  create_wrapper(L, (struct rl_g_buf*) s, RL_SURF_BUF_MT_KEY);

  return 1;
}

int
l_add_point_light(lua_State* L)
{
  struct rl_light_buf* s = check_light_buf(L, 1);

  r_light lgh;

  p31v o;
  c31v c;

  s_get_v31v(o, 2);
  s_get_v31v(c, 3);

  lgh.type = LIGHT_POINT;
  lgh.point.pos = o;
  lgh.point.color = c;

  aarr_push(s->hnd, r_light, &lgh);

  create_wrapper(L, (struct rl_g_buf *) s, RL_LIGHT_BUF_MT_KEY);

  return 1;
}

int
l_add_ambient_light(lua_State* L)
{
  struct rl_light_buf* s = check_light_buf(L, 1);

  r_light lgh;

  c31v o;

  s_get_v31v(o, 2);

  lgh.type = LIGHT_AMBIENT;
  lgh.ambient.color = o;

  aarr_push(s->hnd, r_light, &lgh);

  create_wrapper(L, (struct rl_g_buf *) s, RL_LIGHT_BUF_MT_KEY);

  return 1;
}

int
l_add_phongbling(lua_State* L)
{
  struct rl_material_buf* s = check_material_buf(L, 1);

  r_mtr mtr;

  mtr.type = MTR_PHONG_BLING;

  s_get_v31v(mtr.phong_bling.diff, 2);
  s_get_v31v(mtr.phong_bling.spec, 3);
  mtr.phong_bling.spec_r = s_get_int(4);
  s_get_v31v(mtr.phong_bling.ispec, 5);

  int argc = lua_gettop(L);

  mtr.phong_bling.glossy = 0;
  if (argc >= 6) {
    mtr.phong_bling.glossy = s_get_int(6);
  }

  if (argc >= 7) {
    s_get_v31v(mtr.phong_bling.refr, 7);
  }

  mtr.phong_bling.nidx = 0.0f;
  if (argc >= 8) {
    mtr.phong_bling.nidx = s_get_fp1v(8);
  }

  aarr_push(s->hnd, r_mtr, &mtr);

  create_wrapper(L, (struct rl_g_buf *) s, RL_MATERIAL_BUF_MT_KEY);
  return 1;
}

void
create_wrapper(lua_State* L, struct rl_g_buf* buf, const char* mt_key)
/**
 * Stack effect: -0/+1
 */
{
  struct rl_g_buf* nbuf = lua_newuserdata(L, sizeof(struct rl_g_buf));
  nbuf->hnd = buf->hnd;
  luaL_setmetatable(L, mt_key);
}
