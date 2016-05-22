//
// Created by wo on 5/20/16.
//

#include "lauxlib.h"
#include "lua.h"
#include "lualib.h"
#include "scene.h"
#include "obj_buf.h"

const static struct luaL_Reg rl_scene_funcs[] = {
  {"new", l_create_scene},
  {"render", l_render},
  {"install_camera", l_install_camera},
  {"install_surf_buf", l_install_surf_buf},
  {"surf_buf", l_get_surf_buf},
  {"install_light_buf", l_install_light_buf},
  {"light_buf", l_get_light_buf},
  {"install_material_buf", l_install_material_buf},
  {"material_buf", l_get_material_buf},
  {NULL, NULL}
};

const static struct luaL_Reg rl_camera_funcs[] = {
  {"new", l_create_camera},
  {NULL, NULL}
};

int
__gc_scene(lua_State* L)
{
  struct rl_scene* s = check_scene(L, 1);
  free_scene(s->hnd);

  return 0;
}

int
__gc_camera(lua_State* L)
{
  struct rl_camera* c = check_camera(L, 1);

  /**
   * This function is a stub.
   */

  return 0;
}

void
install_scene_metatables(lua_State* L)
/**
 * This is not a Lua-side function.
 */
{
  luaL_newmetatable(L, RL_SCENE_METATABLE_KEY);
  lua_pushcfunction(L, __gc_scene);
  lua_setfield(L, -2, "__gc");
  luaL_newlib(L, rl_scene_funcs);
  lua_setfield(L, -2, "__index");
  lua_setglobal(L, "RLScene");

  luaL_newmetatable(L, RL_CAMERA_METATABLE_KEY);
  lua_pushcfunction(L, __gc_camera);
  lua_setfield(L, -2, "__gc");
  luaL_newlib(L, rl_camera_funcs);
  lua_setfield(L, -2, "__index");
  lua_setglobal(L, "RLCamera");
}

int
luaopen_scene(lua_State* L)
{
  luaL_newmetatable(L, RL_SCENE_METATABLE_KEY);
  luaL_newlib(L, rl_scene_funcs);

  return 1;
}

int
luaopen_scene_camera(lua_State* L)
{
  luaL_newmetatable(L, RL_CAMERA_METATABLE_KEY);
  luaL_newlib(L, rl_camera_funcs);

  return 1;
}

int
l_create_scene(lua_State* L)
{
  struct rl_surf_buf* surfs = check_surf_buf(L, 1);
  struct rl_light_buf* lights = check_light_buf(L, 2);
  struct rl_material_buf* mtrs = check_material_buf(L, 3);
  struct rl_camera* camera = check_camera(L, 4);

  struct rl_scene* s = lua_newuserdata(L, sizeof(struct rl_scene));

  luaL_setmetatable(L, RL_SCENE_METATABLE_KEY);

  s->hnd->surfs = surfs->hnd;
  s->hnd->lights = lights->hnd;
  s->hnd->materials = mtrs->hnd;
  s->hnd->cam = camera->hnd;

  return 1;
}


int
l_install_camera(lua_State* L)
{
  struct rl_scene* s = check_scene(L, 1);
  struct rl_camera* c = check_camera(L, 2);

  s->hnd->cam = c->hnd;

  return 0;
}

int
l_create_camera(lua_State* L)
{
#define getf(which, idx, key) do {\
  lua_getfield(L, idx, #key);\
  (which).key = (float) lua_tonumber(L, -1);\
  lua_pop(L, 1);\
} while (0)
#define get_v31v(name, idx) do {\
  getf(name, idx, x);\
  getf(name, idx, y);\
  getf(name, idx, z);\
} while (0)
#define get_fp1v(idx) ((float) lua_tonumber(L, idx))

  v31v pos;
  get_v31v(pos, 1);
  v31v dir;
  get_v31v(dir, 2);
  fp1v d = get_fp1v(3);
  fp1v img_w = get_fp1v(4);
  fp1v img_h = get_fp1v(5);
  int pxl_w = (int) lua_tonumber(L, 6);
  int pxl_h = (int) lua_tonumber(L, 7);

  fp1v lens;
  if (lua_isnoneornil(L, 8)) {
    lens = 1e-8f;
  } else {
    lens = get_fp1v(8);
  }

  fp1v aperture_size;
  if (lua_isnoneornil(L, 9)) {
    aperture_size = 1e-8f;
  } else {
    aperture_size = get_fp1v(9);
  }

  struct rl_camera* c = lua_newuserdata(L, sizeof(struct rl_camera));
  make_camera(c->hnd,
              pos, dir,
              d, img_w, img_h,
              pxl_w, pxl_h,
              lens, aperture_size);

  luaL_setmetatable(L, RL_CAMERA_METATABLE_KEY);

  return 1;
}
