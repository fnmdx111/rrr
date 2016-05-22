//
// Created by wo on 5/20/16.
//

#ifndef RRR_RL_SCENE_H
#define RRR_RL_SCENE_H

#include "lua.h"
#include "../rt/scene.h"

struct rl_scene
{
  r_scene* hnd;
};

struct rl_camera
{
  r_camera* hnd;
};

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

void
install_scene_metatables(lua_State* L);

#endif //RRR_SCENE_H
