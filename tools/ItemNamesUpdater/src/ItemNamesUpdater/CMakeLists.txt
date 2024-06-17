add_executable (ItemNamesUpdater "ItemNamesUpdater.cpp" "info.rc")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ItemNamesUpdater PROPERTY CXX_STANDARD 20)
endif()