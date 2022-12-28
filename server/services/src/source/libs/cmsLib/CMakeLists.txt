cmake_minimum_required (VERSION 3.22)

set(PRJ_NAME "cmsLib")

if(${CMAKE_BUILD_TYPE} STREQUAL "Release")
set(PRJ_OUTPUT "cmsLib")
else()
set(PRJ_OUTPUT "cmsLib_d")
endif()

message("${PRJ_NAME} will build ${CMAKE_BUILD_TYPE} version.")

project (${PRJ_NAME})

find_path( NOTSET_SERVICE_DIR curl.h PATHS ${SERVICE_DIR}/src/sdk/include/curl)
if(NOT NOTSET_SERVICE_DIR)
	message(FATAL_ERROR "-DSERVICE_DIR must be set correctly")
endif()

set(CMS_SDKDIR ${SERVICE_DIR}/src/sdk)
set(SRCTOPDIR ${SERVICE_DIR}/src)

set(LIBRARY_OUTPUT_PATH ${SRCTOPDIR}/libs)

add_definitions(-DCURL_STATICLIB)
add_definitions(-D_GLIBCXX_USE_CXX11_ABI=0)

set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -std=c++11 -O0 -Wall -g -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -std=c++11 -O3 -Wall")

file(GLOB_RECURSE src_ls1 ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE rm1 ${CMAKE_CURRENT_SOURCE_DIR}/CMakeCXXCompilerId.cpp)
list(REMOVE_ITEM src_ls1 ${rm1})

file(GLOB_RECURSE src_ls2 ${CMAKE_CURRENT_SOURCE_DIR}/*.c)
file(GLOB_RECURSE rm2 ${CMAKE_CURRENT_SOURCE_DIR}/CMakeCCompilerId.c)
list(REMOVE_ITEM src_ls2 ${rm2})
#message(${src_ls1})

include_directories(${SRCTOPDIR}/source/libs ${CMS_SDKDIR}/include ${CMS_SDKDIR}/include/lua ${CMS_SDKDIR}/include/protobuf/include)

add_library(${PRJ_OUTPUT} STATIC ${src_ls1} ${src_ls2})
