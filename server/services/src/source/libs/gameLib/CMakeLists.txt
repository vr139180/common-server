cmake_minimum_required (VERSION 3.22)

set(PRJ_NAME "gameLib")

project (${PRJ_NAME})

find_path( NOTSET_SERVICE_DIR curl.h PATHS ${SERVICE_DIR}/src/sdk/include/curl)
if(NOT NOTSET_SERVICE_DIR)
	message(FATAL_ERROR "-DSERVICE_DIR must be set correctly")
endif()

set(CMAKE_BUILD_TYPE "Release")

set(CMS_SDKDIR ${SERVICE_DIR}/src/sdk)
set(SRCTOPDIR ${SERVICE_DIR}/src)

set(LIBRARY_OUTPUT_PATH ${SRCTOPDIR}/libs)

add_definitions(-DCURL_STATICLIB)

if(CMAKE_COMPILER_IS_GNUCXX)
	set(CMAKE_CXX_FLAGS "-std=c++11 -O2 -Wall ${CMAKE_CXX_FLAGS}")
endif(CMAKE_COMPILER_IS_GNUCXX)

file(GLOB_RECURSE src_ls1 ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE src_ls2 ${CMAKE_CURRENT_SOURCE_DIR}/*.cc)

file(GLOB_RECURSE rm1 ${CMAKE_CURRENT_SOURCE_DIR}/CMakeCXXCompilerId.cpp)
#file(GLOB_RECURSE rm2 ${CMAKE_CURRENT_SOURCE_DIR}/CMakeCCompilerId.c)

list(REMOVE_ITEM src_ls1 ${rm1})

include_directories(${SRCTOPDIR}/source/libs ${CMS_SDKDIR}/include ${CMS_SDKDIR}/include/lua ${CMS_SDKDIR}/include/protobuf/include)

add_library(${PRJ_NAME} STATIC ${src_ls1} ${src_ls2})
