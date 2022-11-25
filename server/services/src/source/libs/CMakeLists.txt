cmake_minimum_required (VERSION 3.22)

set(PRJ_NAME "libs")

project (${PRJ_NAME})

find_path( NOTSET_SERVICE_DIR curl.h PATHS ${SERVICE_DIR}/src/sdk/include/curl)
if(NOT NOTSET_SERVICE_DIR)
	message(FATAL_ERROR "-DSERVICE_DIR must be set correctly")
endif()

add_subdirectory(cmsLib)

add_subdirectory(gameLib)

add_subdirectory(taskLib)

add_subdirectory(worldsLib)
