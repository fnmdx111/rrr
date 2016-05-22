#include <stdio.h>
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
#include "libs/scripting/scene.h"
#include "libs/scripting/obj_buf.h"
#include "libs/math/vcommon.h"

const static struct luaL_Reg static_init_funcs[] = {
  {"create_scene", l_create_scene},
  {"create_camera", l_create_camera},
  {"append_wavefront_obj", l_append_wavefront_object},
  {NULL, NULL}
};

void
rl_static_init(lua_State* L)
{
  install_scene_metatables(L);
  install_buf_metatables(L);
}

int
main(int argc, char** argv)
{
  lua_State* L = luaL_newstate();
  luaL_openlibs(L);
  rl_static_init(L);

  luaL_dofile(L, argv[1]);

  lua_getglobal(L, "scene");
  struct rl_scene* s = check_scene(L, -1);
  p_v34v(&s->hnd->cam->dir);

  lua_close(L);

  return 0;
}
