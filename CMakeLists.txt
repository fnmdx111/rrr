cmake_minimum_required(VERSION 3.5)
project(rrr)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c11")
find_package(Lua52 REQUIRED)
find_package(SSE REQUIRED)

set(SOURCE_FILES main.c ray_pk4.c ray_pk4.h libs/vecp.c libs/vecp.h surfaces.c surfaces.h libs/vec.c libs/vec.h libs/portable.h scene.c scene.h lights.c lights.h libs/generic/aarr.c libs/generic/aarr.h aarr_test.c)
set(AARR_SOURCE_FILES aarr_test.c libs/generic/aarr.c libs/generic/aarr.h)
add_executable(rrr ${SOURCE_FILES})
add_executable(aarr ${AARR_SOURCE_FILES})

target_compile_definitions(aarr PRIVATE)