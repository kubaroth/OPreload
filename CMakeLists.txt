cmake_minimum_required(VERSION 3.13.0)

project(houdini_reload)

set(HOUDINI_VERSION_MAJOR 18)
set(HOUDINI_VERSION_MINOR 0)
set(HOUDINI_VERSION_PATCH 416)

set(HFS $ENV{HOME}/hfs18.0.416)

message ("HOUDINI VERSION: " $ENV{HOUDINI_VERSION_MAJOR}.$ENV{HOUDINI_VERSION_MINOR}.$ENV{HOUDINI_VERSION_PATCH})
set (HOUDINI_VERSION $ENV{HOUDINI_VERSION_MAJOR}.$ENV{HOUDINI_VERSION_MINOR}.$ENV{HOUDINI_VERSION_PATCH} )


add_definitions(
       -DPLUGIN_DIR="${CMAKE_BINARY_DIR}"
       -DVERSION=\"${HOUDINI_VERSION}\"
       -DHOUDINI_VERSION=${HOUDINI_VERSION}
       -DHOUDINI_MAJOR_RELEASE=${HOUDINI_VERSION_MAJOR}
       -DHOUDINI_MINOR_RELEASE=${HOUDINI_VERSION_MINOR}
       -DHOUDINI_BUILD_VERSION=${HOUDINI_VERSION_PATCH}
       -D_GNU_SOURCE
       -DLINUX
       -DAMD64
       -DSIZEOF_VOID_P=8
       -DFBX_ENABLED=1
       -DOPENCL_ENABLED=1
       -DOPENVDB_ENABLED=1
       -DSESI_LITTLE_ENDIAN
       -DENABLE_THREADS
       -DUSE_PTHREADS
       -DMAKING_DSO
       -D_REENTRANT
       -D_FILE_OFFSET_BITS=64
       -DGCC4
       -DGCC3
       )

set(HOUDINI_LIBRARIES
       HoudiniUI
       HoudiniUT
       HoudiniOPZ
       HoudiniOP1
       HoudiniOP2
       HoudiniOP3
       HoudiniSIM
       HoudiniGEO
       HoudiniPRM
       HoudiniRAY
       hboost_system
       tbb
       
       # GLU 
       # GL 
       # X11 
       # Xext 
       # Xi 
       # dl 
       pthread
       )

set(CMAKE_CXX_FLAGS "-fopenmp")
     
add_library(${PROJECT_NAME} SHARED
  star.cpp
  )

target_include_directories(${PROJECT_NAME} BEFORE PUBLIC
  ${HFS}/toolkit/include/
  )

target_link_directories(${PROJECT_NAME} BEFORE PUBLIC ${HFS}/dsolib)

target_link_libraries(${PROJECT_NAME} ${HOUDINI_LIBRARIES})

install(TARGETS ${PROJECT_NAME} DESTINATION dso)


### Guest
set (PLUGIN guest_plugin)

add_library(${PLUGIN} SHARED
  guest_plugin.cpp
  )

target_include_directories(${PLUGIN} BEFORE PUBLIC
  ${HFS}/toolkit/include/
  )

target_link_directories(${PLUGIN} BEFORE PUBLIC ${HFS}/dsolib)

target_link_libraries(${PLUGIN} ${HOUDINI_LIBRARIES})

# Currenlty not used as the plugin is hardcoded to build path
#install(TARGETS ${PLUGIN} DESTINATION dso)

