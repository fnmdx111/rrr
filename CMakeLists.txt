cmake_minimum_required(VERSION 3.5)
project(rrr)

find_package(Lua52 REQUIRED)
find_package(SSE REQUIRED)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11 -msse4.2")

set(SOURCE_FILES main.c libs/rt/ray_pk4.c libs/rt/ray_pk4.h libs/math/vecp.c libs/math/vecp.h libs/rt/surfaces.c libs/rt/surfaces.h libs/math/vec.c libs/math/vec.h libs/portable.h libs/rt/scene.c libs/rt/scene.h libs/rt/lights.c libs/rt/lights.h libs/generic/aarr.c libs/generic/aarr.h libs/math/vcommon.h libs/math/vcommon.c)
set(AARR_SOURCE_FILES aarr_test.c libs/generic/aarr.c libs/generic/aarr.h libs/scripting/obj_buf.c libs/scripting/obj_buf.h libs/scripting/scene.c libs/scripting/scene.h)
add_executable(rrr ${SOURCE_FILES})
add_executable(aarr ${AARR_SOURCE_FILES})

target_compile_definitions(aarr PRIVATE)
target_link_libraries(rrr PRIVATE -llua -lm)
