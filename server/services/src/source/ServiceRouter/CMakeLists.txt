cmake_minimum_required (VERSION 3.22)

set(PRJ_NAME "ServiceRouter")

project (${PRJ_NAME})

find_path( NOTSET_SERVICE_DIR curl.h PATHS ${SERVICE_DIR}/src/sdk/include/curl)
if(NOT NOTSET_SERVICE_DIR)
	message(FATAL_ERROR "-DSERVICE_DIR must be set correctly")
endif()

set(CMAKE_BUILD_TYPE "Release")

set(CMS_SDKDIR ${SERVICE_DIR}/src/sdk)
set(SRCTOPDIR ${SERVICE_DIR}/src)

set(EXECUTABLE_OUTPUT_PATH ${SERVICE_DIR}/server/${PRJ_NAME})

add_definitions(-DCURL_STATICLIB)

if(CMAKE_COMPILER_IS_GNUCXX)
	#set(CMAKE_CXX_FLAGS "-std=c++11 -O2 -Wall ${CMAKE_CXX_FLAGS}")
	set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")
endif(CMAKE_COMPILER_IS_GNUCXX)

file(GLOB_RECURSE src_ls1 ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE rm1 ${CMAKE_CURRENT_SOURCE_DIR}/CMakeCXXCompilerId.cpp)
list(REMOVE_ITEM src_ls1 ${rm1})

include_directories(${SRCTOPDIR}/source/${PRJ_NAME} ${SRCTOPDIR}/source/libs ${CMS_SDKDIR}/include ${CMS_SDKDIR}/include/lua ${CMS_SDKDIR}/include/protobuf/include)

link_directories(${CMS_SDKDIR}/linuxlib ${SRCTOPDIR}/libs)

add_executable(${PRJ_NAME} ${src_ls1})

target_link_libraries(${PRJ_NAME} -Wl,--start-group libcurl.a libredis++.a libhiredis.a libtinyxml2.a libprotobuf.a libz.a
 libboost_atomic.a libboost_chrono.a libboost_container.a libboost_context.a libboost_contract.a libboost_coroutine.a 
 libboost_date_time.a libboost_exception.a libboost_filesystem.a libboost_iostreams.a libboost_log.a libboost_log_setup.a 
 libboost_program_options.a libboost_random.a libboost_regex.a libboost_serialization.a libboost_system.a libboost_thread.a 
 libboost_timer.a libboost_type_erasure.a libboost_wserialization.a 
 libcmsLib.a libgameLib.a -lpthread -ldl -lssl -lcrypto
 -Wl,--end-group)
