cmake_minimum_required (VERSION 3.22)

set(PRJ_NAME "services")

project (${PRJ_NAME})

find_path( NOTSET_SERVICE_DIR curl.h PATHS ${SERVICE_DIR}/src/sdk/include/curl)
if(NOT NOTSET_SERVICE_DIR)
	message(FATAL_ERROR "-DSERVICE_DIR must be set correctly")
endif()

add_subdirectory(libs)

add_subdirectory(sEureka)
add_subdirectory(GateService)
add_subdirectory(HomeService)
add_subdirectory(RouterService)
add_subdirectory(UnionService)
add_subdirectory(GameService)
