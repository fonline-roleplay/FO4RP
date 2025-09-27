cmake_minimum_required(VERSION 4.0.0)
project(fonline_tnf_client64)

# Set defines and optimization
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -D__CLIENT")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS}")

# Create shared library target
add_library(fonline_tnf_client64 SHARED
    fonline_tnf.cpp
    qmap_tools.cpp
    q_sprites.cpp
)

# Set output name and properties
set_target_properties(fonline_tnf_client64 PROPERTIES
    OUTPUT_NAME "fonline_tnf_client64"
    SUFFIX ".so"
    PREFIX ""
    POSITION_INDEPENDENT_CODE ON
)

# Link standard libraries
target_link_libraries(fonline_tnf_client64 PRIVATE stdc++)
