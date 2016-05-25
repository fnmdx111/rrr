cmake_minimum_required(VERSION 3.5)
project(rrr)

find_package(Lua52 REQUIRED)
find_package(SSE REQUIRED)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11 -msse4.2")

set(SCRIPTING_SRC_FILES libs/scripting/scene.c libs/scripting/scene.h libs/scripting/obj_buf.c libs/scripting/obj_buf.h libs/scripting/scommon.h.h)
set(MATH_SRC_FILES libs/math/vecp.c libs/math/vecp.h libs/math/vec.c libs/math/vec.h libs/math/vcommon.c libs/math/vcommon.h libs/math/common.h libs/math/common.c)
set(GENERIC_SRC_FILES libs/generic/aarr.c libs/generic/aarr.h)
set(RT_SRC_FILES libs/rt/ray_pk4.c libs/rt/ray_pk4.h libs/rt/surfaces.c libs/rt/surfaces.h libs/rt/scene.c libs/rt/scene.h libs/rt/lights.c libs/rt/lights.h)
set(SOURCE_FILES main.c libs/portable.h ${RT_SRC_FILES} ${MATH_SRC_FILES} ${GENERIC_SRC_FILES} ${SCRIPTING_SRC_FILES})
set(AARR_SOURCE_FILES aarr_test.c ${GENERIC_SRC_FILES})
add_executable(rrr ${SOURCE_FILES})
add_executable(aarr ${AARR_SOURCE_FILES})

target_compile_definitions(aarr PRIVATE)
target_link_libraries(rrr PRIVATE -llua -lm)
