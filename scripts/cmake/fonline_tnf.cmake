cmake_minimum_required(VERSION 4.0.0)
project(fonline_tnf)

# Set 32-bit architecture, defines and optimization
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -m32")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m32 -O2 -D__SERVER")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -m32")

# Create shared library target
add_library(fonline_tnf SHARED
    fonline_tnf.cpp
    qmap_tools.cpp
)

# Set output name and properties
set_target_properties(fonline_tnf PROPERTIES
    OUTPUT_NAME "fonline_tnf"
    SUFFIX ".so"
    PREFIX ""
    POSITION_INDEPENDENT_CODE ON
)

# Link standard libraries
target_link_libraries(fonline_tnf PRIVATE stdc++)
