//
// Created by wo on 5/24/16.
//

#ifndef RRR_SCOMMON_H_H
#define RRR_SCOMMON_H_H

#define s_getf(which, idx, key) do {\
  lua_getfield(L, idx, #key);\
  (which).key = (float) lua_tonumber(L, -1);\
  lua_pop(L, 1);\
} while (0)
#define s_get_v31v(name, idx) do {\
  s_getf(name, idx, x);\
  s_getf(name, idx, y);\
  s_getf(name, idx, z);\
} while (0)
#define s_get_fp1v(idx) ((float) lua_tonumber(L, idx))
#define s_get_int(idx) ((int) lua_tonumber(L, idx))

#endif //RRR_SCOMMON_H_H
