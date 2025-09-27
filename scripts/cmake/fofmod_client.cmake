cmake_minimum_required(VERSION 4.0.0)
project(fofmod_client)

# fmod - release version
# fmodL - debug version

# readelf -l libfmod.so.11.22 shows executable flag on GNU_STACK, which results in runtime error
# "cannot enable executable stack as shared object requires: Invalid argument"
# to fix it I did
# yay -S execstack
# execstack -c libfmodL.so.11.22 libfmod.so.11.22

# Set 32-bit architecture, defines and optimization
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -m32")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m32 -O2 -D__CLIENT -DFOFMOD_DEBUG")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -m32")

# Create shared library target
add_library(fofmod_client SHARED
    fmod/src/cwalk/cwalk.c
    fmod/inc/miniz/miniz.c
    fmod/src/fofmodlock.cpp
    fmod/src/fofmodthread.cpp
    fmod/src/perfcounter.cpp
    fmod/src/zipfile.cpp
    fmod/src/archive.cpp
    fmod/src/util.cpp
    fmod/src/fofmod.cpp
    fmod/src/fofmoddsp.cpp
    fmod/src/fofmoddspAS.cpp
    fmod/src/fofmodchannelcontrol.cpp
    fmod/src/fofmodchannelgroup.cpp
    fmod/src/cachedata.cpp
    fmod/src/fofmodlistener.cpp
    fmod/src/fofmodsystem.cpp
    fmod/src/fofmodsound.cpp
    fmod/src/fofmodsoundAS.cpp
    fmod/src/fofmodchannel.cpp
    fmod/src/fofmodchannelAS.cpp
    fmod/src/refcount.cpp
)

target_include_directories(fofmod_client PRIVATE
    ./
    fmod/inc
    fmod/inc/tinydir
    fmod/inc/fmod
    fmod/inc/miniz
    fmod/inc/cwalk
)

target_link_directories(fofmod_client PRIVATE
    fmod/lib/fmod
)

# Set output name and properties
set_target_properties(fofmod_client PROPERTIES
    OUTPUT_NAME "fofmod_client"
    SUFFIX ".so"
    PREFIX ""
    POSITION_INDEPENDENT_CODE ON
)

# Link standard libraries and libfmod
target_link_libraries(fofmod_client PRIVATE
    stdc++
    fmod
)

set_target_properties(fofmod_client
    PROPERTIES
        INSTALL_RPATH "$ORIGIN/fmod/lib/fmod"
        BUILD_WITH_INSTALL_RPATH true
        LINK_OPTIONS "-Wl,--disable-new-dtags"
)
