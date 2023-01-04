cmake_minimum_required (VERSION 3.22)

set(PRJ_NAME "StateService")

if(${CMAKE_BUILD_TYPE} STREQUAL "Release")
set(PRJ_OUTPUT "StateService")
set(CMSLIB_NAME "libcmsLib.a")
set(GAMELIB_NAME "libgameLib.a")
else()
set(PRJ_OUTPUT "StateService_d")
set(CMSLIB_NAME "libcmsLib_d.a")
set(GAMELIB_NAME "libgameLib_d.a")
endif()

message("${PRJ_NAME} will build ${CMAKE_BUILD_TYPE} version.")

project (${PRJ_NAME})

find_path( NOTSET_SERVICE_DIR curl.h PATHS ${SERVICE_DIR}/src/sdk/include/curl)
if(NOT NOTSET_SERVICE_DIR)
	message(FATAL_ERROR "-DSERVICE_DIR must be set correctly")
endif()

set(MYSQL_H /usr/include/mysql)
set(MYSQL_LIB /usr/lib64/mysql)

set(CMS_SDKDIR ${SERVICE_DIR}/src/sdk)
set(SRCTOPDIR ${SERVICE_DIR}/src)

set(EXECUTABLE_OUTPUT_PATH ${SERVICE_DIR}/server/${PRJ_NAME})

add_definitions(-DCURL_STATICLIB)
add_definitions(-D_GLIBCXX_USE_CXX11_ABI=0)

set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -std=c++11 -O0 -Wall -g -ggdb -fno-omit-frame-pointer")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -std=c++11 -O3 -Wall -fno-omit-frame-pointer")

file(GLOB_RECURSE src_ls1 ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE rm1 ${CMAKE_CURRENT_SOURCE_DIR}/CMakeCXXCompilerId.cpp)
list(REMOVE_ITEM src_ls1 ${rm1})

include_directories(${SRCTOPDIR}/source/${PRJ_NAME} ${SRCTOPDIR}/source/libs ${CMS_SDKDIR}/include ${CMS_SDKDIR}/include/lua ${CMS_SDKDIR}/include/protobuf/include ${MYSQL_H})

link_directories(${CMS_SDKDIR}/linuxlib ${SRCTOPDIR}/libs ${MYSQL_LIB})

add_executable(${PRJ_OUTPUT} ${src_ls1})

target_link_libraries(${PRJ_OUTPUT} -Wl,--start-group libcurl.a libmysqlcppconn-static.a libredis++.a libhiredis.a libtinyxml2.a libprotobuf.a
 libboost_atomic.a libboost_chrono.a libboost_container.a libboost_context.a libboost_contract.a libboost_coroutine.a libz.a
 libboost_date_time.a libboost_exception.a libboost_filesystem.a libboost_iostreams.a libboost_log.a libboost_log_setup.a 
 libboost_program_options.a libboost_random.a libboost_regex.a libboost_serialization.a libboost_system.a libboost_thread.a 
 libboost_timer.a libboost_type_erasure.a libboost_wserialization.a libtcmalloc_minimal.a
 ${CMSLIB_NAME} ${GAMELIB_NAME} -lpthread -ldl -lssl -lcrypto -lmysqlclient
 -Wl,--end-group)
