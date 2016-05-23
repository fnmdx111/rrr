#include <stdio.h>
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
#include "libs/scripting/scene.h"
#include "libs/scripting/obj_buf.h"
#include "libs/math/vcommon.h"
#include <stdlib.h>
#include <string.h>

const static struct luaL_Reg static_init_funcs[] = {
  {"create_scene", l_create_scene},
  {"create_camera", l_create_camera},
  {"append_wavefront_obj", l_append_wavefront_object},
  {NULL, NULL}
};

static void*
aalloc(void* ud, void* ptr, size_t osize, size_t nsize)
{
  (void) ud;
  void* ret;

  if (nsize == 0) {
    free(ptr);
    ret = NULL;
  } else {
    int err = posix_memalign(&ret, 16, nsize);
    // printf("new addr is %p\n", ret);

    if (err) {
      fprintf(stderr, "posix_memalign error\n");
    }

    if (ptr != NULL) {
      memmove(ret, ptr, osize > nsize ? nsize : osize);

      free(ptr);
    }
  }

  return ret;
}

void*
l_alloc(void* ud, void* ptr, size_t osize, size_t nsize)
{
  if (nsize == 0) {
    free(ptr);
    return NULL;
  } else {
    return realloc(ptr, nsize);
  }
}

void
rl_static_init(lua_State* L)
{
  install_scene_metatables(L);
  install_buf_metatables(L);
}

int
atpanic(lua_State* L)
{
  printf("atpanic: %s\n", lua_tostring(L, -1));

  return 0;
}

int
main(int argc, char** argv)
{
  lua_State* L = lua_newstate(aalloc, NULL);
  lua_atpanic(L, atpanic);

  luaL_openlibs(L);
  rl_static_init(L);

  luaL_loadfile(L, argv[1]);
  int err = lua_pcall(L, 0, LUA_MULTRET, 0);

  if (err) {
    if (lua_type(L, -1) == LUA_TSTRING) {
      fprintf(stderr, "%s\n", lua_tostring(L, -1));
      return 1;
    }
  } else {
    printf("config ok\n");
  }

  lua_getglobal(L, "scene");
  if (lua_type(L, -1) == LUA_TSTRING) {
    printf("%s\n", lua_tostring(L, -1));
  }

  struct rl_scene* s = check_scene(L, -1);
  p_v34v(&(s->hnd.cam->dir));

  lua_close(L);

  return 0;
}
