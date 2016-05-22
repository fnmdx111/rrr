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
__gc_buf(lua_State* L)
{
  struct rl_g_buf* g = lua_touserdata(L, 1);
  free_aarr(g->hnd);

  return 0;
}

const static struct luaL_Reg rl_surf_buf_funcs[] = {
  {"new", l_create_surf_buf},
  {"append_wavefront_object", l_append_wavefront_object},
  {NULL, NULL}
};

const static struct luaL_Reg rl_light_buf_funcs[] = {
  {"new", l_create_light_buf},
  {NULL, NULL}
};

const static struct luaL_Reg rl_mtr_buf_funcs[] = {
  {"new", l_create_material_buf},
  {NULL, NULL}
};

void
install_buf_metatables(lua_State* L)
{
  luaL_newmetatable(L, RL_SURF_BUF_MT_KEY);
  lua_pushcfunction(L, __gc_buf);
  lua_setfield(L, -2, "__gc");
  luaL_newlib(L, rl_surf_buf_funcs);
  lua_setfield(L, -2, "__index");
  lua_setglobal(L, "RLSurfaceBuf");

  luaL_newmetatable(L, RL_LIGHT_BUF_MT_KEY);
  lua_pushcfunction(L, __gc_buf);
  lua_setfield(L, -2, "__gc");
  luaL_newlib(L, rl_light_buf_funcs);
  lua_setfield(L, -2, "__index");
  lua_setglobal(L, "RLLightBuf");

  luaL_newmetatable(L, RL_MATERIAL_BUF_MT_KEY);
  lua_pushcfunction(L, __gc_buf);
  lua_setfield(L, -2, "__gc");
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
  make_aarr(surf->hnd, sizeof(r_surf), 0);

  luaL_setmetatable(L, RL_SURF_BUF_MT_KEY);
  return 1;
}

int
l_create_light_buf(lua_State* L)
{
  struct rl_light_buf* light = lua_newuserdata(L, sizeof(struct rl_light_buf));
  make_aarr(light->hnd, sizeof(r_light), 0);

  luaL_setmetatable(L, RL_LIGHT_BUF_MT_KEY);
  return 1;
}

int
l_create_material_buf(lua_State* L)
{
  struct rl_material_buf* mtr = lua_newuserdata(L,
                                                sizeof(struct rl_material_buf));
  make_aarr(mtr->hnd, sizeof(r_mtr), 0);

  luaL_setmetatable(L, RL_LIGHT_BUF_MT_KEY);
  return 1;
}

int
l_append_wavefront_object(lua_State *L)
{
  struct rl_surf_buf* s = check_surf_buf(L, 1);
  const char* fn = lua_tostring(L, 2);
  append_wavefront_object(s->hnd, fn);

  return 0;
}
