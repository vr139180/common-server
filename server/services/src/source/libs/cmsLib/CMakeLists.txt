cmake_minimum_required (VERSION 3.22)

set(PRJ_NAME "cmsLib")

project (${PRJ_NAME})

find_path( NOTSET_SERVICE_DIR curl.h PATHS ${SERVICE_DIR}/src/sdk/include/curl)
if(NOT NOTSET_SERVICE_DIR)
	message(FATAL_ERROR "-DSERVICE_DIR must be set correctly")
endif()

set(CMAKE_BUILD_TYPE "Release")

set(CMS_SDKDIR ${SERVICE_DIR}/src/sdk)
set(SRCTOPDIR ${SERVICE_DIR}/src)

set(LIBRARY_OUTPUT_PATH ${SRCTOPDIR}/libs)

set(CURL_STATICLIB ON)
add_compile_options(-std=c++11)
#set(CMAKE_CXX_FLAGS "-std=c+11" ${CMAKE_CXX_FLAGS})

file(GLOB_RECURSE src_ls1 ${CMAKE_CURRENT_SOURCE_DIR}/**/*.cpp)
file(GLOB_RECURSE src_ls2 ${CMAKE_CURRENT_SOURCE_DIR}/**/*.c)
file(GLOB_RECURSE thirdlibs ${CMS_SDKDIR}/linuxlib/*.a)

include_directories(${SRCTOPDIR}/source/libs ${CMS_SDKDIR}/include ${CMS_SDKDIR}/include/lua ${CMS_SDKDIR}/include/protobuf/include)

add_library(${PRJ_NAME} STATIC ${src_ls1} ${src_ls2})
