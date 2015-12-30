
cmake_minimum_required(VERSION 2.6)

project(executable)

FIND_PACKAGE(GLEW REQUIRED)
FIND_PACKAGE(GLUT REQUIRED)
FIND_PACKAGE(OpenGL REQUIRED)

INCLUDE_DIRECTORIES(${GLEW_INCLUDE_DIRS})
LINK_DIRECTORIES(${GLEW_LIBRARY_DIRS})

INCLUDE_DIRECTORIES(${GLUT_INCLUDE_DIR})
LINK_DIRECTORIES(${GLUT_LIBRARY_DIRS})

INCLUDE_DIRECTORIES(${OPENGL_INCLUDE_DIR})


file(
GLOB
source_files
src/*.[ch]pp src/shader*
src/format/*.[ch]pp
)

SET(CMAKE_BUILD_TYPE Debug)
SET(CMAKE_CXX_FLAGS "-O2 -Wall -Wextra" )
SET(CMAKE_CXX_COMPILER "g++")



add_executable(	
   executable
   ${source_files}
   )	

## Nvidia setting for CPE installs
LINK_DIRECTORIES("/usr/lib/nvidia-331")

target_link_libraries(executable ${GLUT_LIBRARIES} ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES})


