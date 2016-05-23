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

const struct luaL_Reg rl_surf_buf_funcs[] = {
  {"append_wavefront_object", l_append_wavefront_object},
  {"add_surface", l_add_surface},
  {"close", close_buf},
  {NULL, NULL}
};

const struct luaL_Reg rl_light_buf_funcs[] = {
  {"close", close_buf},
  {NULL, NULL}
};

const struct luaL_Reg rl_mtr_buf_funcs[] = {
  {"close", close_buf},
  {NULL, NULL}
};

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
l_add_surface(lua_State* L)
{
  struct rl_surf_buf* s = check_surf_buf(L, 1);
  create_wrapper(L, (struct rl_g_buf *) s, RL_SURF_BUF_MT_KEY);

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
