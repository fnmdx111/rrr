//
// Created by wo on 5/20/16.
//

#ifndef RRR_RL_SCENE_H
#define RRR_RL_SCENE_H

#include "lua.h"
#include "../rt/scene.h"
#include <stdint.h>

struct rl_scene
{
  r_scene hnd;
};

struct rl_camera
{
  int64_t ___;
  r_camera hnd;
  /**
   * NEVER access this field DIRECTLY!
   * Use the following macro to access the stored r_camera struct.
   *
   * This is really ugly:
   * By defining the custom Lua memory allocator, I've managed to force
   * it to align objects' addresses to 0xXXX0 (16-byte alignment), which
   * is essential for SSE instructions (_mm_set1_ps for example).
   * Lua, however, generates new userdata for 0x28 more bytes, making some
   * userdata not aligned in 16-byte.
   *
   * To get around this, I first tried inserting a 8-byte object in front
   * of `hnd`, which didn't work because the compiler automatically aligns
   * the 8-byte object to 16-byte, rendering the pre-padding invalid.
   *
   * So I have to use a dirtier hack, accessing `hnd` by offsetting 8 bytes:
   *   hnd_addr = (r_camera*) (((int64_t*) rl_camera_addr) + 1)
   *
   * Because raw Lua object aligns to 0xXYZ0, rl_camera_aligns to 0xPQR8,
   * r_camera aligns to 0xUVW0.
   *
   * Further investigation shows that, Lua allocates sizeof(Udata) + s for a
   * new userdata of size s. The Udata part is a CommonHeader, which is
   * apparently a header for garbage collection (see lobject.h:GCheader) of
   * size 10 bytes.
   *
   * The whole Udata is unioned with an L_Umaxalign object and a uv struct which
   * contains three more 8 bytes (i.e. 24 bytes). The total size of the uv
   * struct is 34 bytes. After aligning with the L_Umaxalign object, the size of
   * a Udata becomes 40 (apparently the system is aligned to 8 bytes).
   *
   * To eliminate this hack, you have to recompile Lua and make sure the Udata
   * union is aligned to 16 bytes.
   */
};

#define SHOULD_SHIFT 1
#define RL_CAMP(rc_p) ((r_camera*) ((int64_t*)(rc_p) + SHOULD_SHIFT))

#define RL_SCENE_METATABLE_KEY "rrr.rl_scene"
#define RL_CAMERA_METATABLE_KEY "rrr.rl_camera"
#define check_scene(L, i) ((struct rl_scene*) luaL_checkudata(L, i, RL_SCENE_METATABLE_KEY))
#define check_camera(L, i) ((struct rl_camera*) luaL_checkudata(L, i, RL_CAMERA_METATABLE_KEY))

int
l_initialize(lua_State* L);

int
l_create_scene(lua_State* L);

int
l_create_camera(lua_State* L);

int
l_render(lua_State* L);

int
l_install_camera(lua_State* L);

int
l_install_surf_buf(lua_State* L);

int
l_install_light_buf(lua_State* L);

int
l_install_material_buf(lua_State* L);

int
l_get_surf_buf(lua_State* L);

void
install_scene_metatables(lua_State* L);

#endif //RRR_SCENE_H
