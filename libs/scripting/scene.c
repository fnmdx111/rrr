//
// Created by wo on 5/20/16.
//

#include "lauxlib.h"
#include "lua.h"
#include "lualib.h"
#include "scene.h"
#include "obj_buf.h"
#include "scommon.h"

const static struct luaL_Reg rl_scene_funcs[] = {
  {"render", l_render},
  {"install_camera", l_install_camera},
  {"install_surf_buf", l_install_surf_buf},
  {"surf_buf", l_get_surf_buf},
  {"install_light_buf", l_install_light_buf},
//  {"light_buf", l_get_light_buf},
  {"install_material_buf", l_install_material_buf},
//  {"material_buf", l_get_material_buf},
  {NULL, NULL}
};

const static struct luaL_Reg rl_camera_funcs[] = {
  {NULL, NULL}
};

int
__gc_scene(lua_State* L)
{
  struct rl_scene* s = check_scene(L, 1);
  free_scene(&s->hnd);

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
  /**
   * RLScene initialization
   */
  luaL_newmetatable(L, RL_SCENE_METATABLE_KEY);

  lua_pushcfunction(L, __gc_scene);
  lua_setfield(L, -2, "__gc");

  lua_pushcfunction(L, l_create_scene);
  lua_setfield(L, -2, "new");

  luaL_newlib(L, rl_scene_funcs);
  lua_setfield(L, -2, "__index");
  lua_setglobal(L, "RLScene");

  /**
   * RLCamera initialization
   */
  luaL_newmetatable(L, RL_CAMERA_METATABLE_KEY);

  lua_pushcfunction(L, __gc_camera);
  lua_setfield(L, -2, "__gc");

  lua_pushcfunction(L, l_create_camera);
  lua_setfield(L, -2, "new");

  luaL_newlib(L, rl_camera_funcs);
  lua_setfield(L, -2, "__index");
  lua_setglobal(L, "RLCamera");
}

int
l_get_surf_buf(lua_State* L)
{
  struct rl_scene* s = check_scene(L, 1);
  create_wrapper(L, (struct rl_g_buf *) s, RL_SURF_BUF_MT_KEY);

  return 1;
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
  int argc = lua_gettop(L);

  struct rl_surf_buf* surfs = NULL;
  struct rl_light_buf* lights = NULL;
  struct rl_material_buf* mtrs = NULL;
  struct rl_camera* camera = NULL;

  if (argc == 4) {
    surfs = check_surf_buf(L, 1);
    lights = check_light_buf(L, 2);
    mtrs = check_material_buf(L, 3);
    camera = check_camera(L, 4);
  } else if (argc == 3) {
    surfs = check_surf_buf(L, 1);
    lights = check_light_buf(L, 2);
    mtrs = check_material_buf(L, 3);
  } else if (argc == 1) {
    camera = check_camera(L, 1);
  }

  struct rl_scene* s = lua_newuserdata(L, sizeof(struct rl_scene));

  luaL_setmetatable(L, RL_SCENE_METATABLE_KEY);

  if (surfs && lights && mtrs) {
    s->hnd.surfs = surfs->hnd;
    s->hnd.lights = lights->hnd;
    s->hnd.materials = mtrs->hnd;
  }

  if (camera) {
    s->hnd.cam = RL_CAMP(camera);
  }

  return 1;
}


int
l_install_camera(lua_State* L)
{
  struct rl_scene* s = check_scene(L, 1);
  struct rl_camera* c = check_camera(L, 2);

  s->hnd.cam = RL_CAMP(c);

  return 0;
}

int
l_install_surf_buf(lua_State* L)
{
  struct rl_scene* s = check_scene(L, 1);
  struct rl_surf_buf* buf = check_surf_buf(L, 2);

  s->hnd.surfs = buf->hnd;

  return 0;
}

int
l_install_light_buf(lua_State* L)
{
  struct rl_scene* s = check_scene(L, 1);
  struct rl_light_buf* buf = check_light_buf(L, 2);

  s->hnd.lights = buf->hnd;

  return 0;
}

int
l_install_material_buf(lua_State* L)
{
  struct rl_scene* s = check_scene(L, 1);
  struct rl_material_buf* buf = check_material_buf(L, 2);

  s->hnd.materials = buf->hnd;

  return 0;
}

int
l_create_camera(lua_State* L)
{
  v31v pos;
  s_get_v31v(pos, 1);
  v31v dir;
  s_get_v31v(dir, 2);
  fp1v d = s_get_fp1v(3);
  fp1v img_w = s_get_fp1v(4);
  fp1v img_h = s_get_fp1v(5);
  int pxl_w = (int) lua_tonumber(L, 6);
  int pxl_h = (int) lua_tonumber(L, 7);

  fp1v lens = 1e-8;
  fp1v aperture_size = 1e-8;

  int topn = lua_gettop(L);
  if (topn >= 9) {
    aperture_size = s_get_fp1v(9);
  }
  if (topn >= 8) {
    lens = s_get_fp1v(8);
  }

  struct rl_camera* c = lua_newuserdata(L, sizeof(struct rl_camera));
  make_camera(RL_CAMP(c),
              pos, dir,
              d, img_w, img_h,
              pxl_w, pxl_h,
              lens, aperture_size);

  luaL_setmetatable(L, RL_CAMERA_METATABLE_KEY);

  return 1;
}

int
l_render(lua_State* L)
{
  struct rl_scene* s = check_scene(L, 1);
  render(&s->hnd);

  return 1;
}