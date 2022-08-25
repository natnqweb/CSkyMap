# CMakeList.txt : CMake project for CSkyMap, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("CSkyMap")

# Add source to this project's executable.
add_executable (CSkyMap "CSkyMap.c" "SkyMap.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET CSkyMap PROPERTY C_STANDARD)
endif()

# TODO: Add tests and install targets if needed.
