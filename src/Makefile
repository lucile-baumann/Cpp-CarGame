CXXFLAGS= -g -Wall -Wextra -O2
LDFLAGS = -L/usr/lib/nvidia-331 -lm -lglut -lGLEW -lGLU -lGL -pthread
#remarque: /usr/lib/nvidia-331 specifique pour les installations de CPE


all: executable

executable: main.o glutils.o mat4.o vec2.o vec3.o triangle_index.o vertex_opengl.o tga.o mesh.o format/mesh_io_obj.o format/mesh_io_off.o
	g++ $^ -o executable $(LDFLAGS)

main.o: main.cpp glutils.hpp mat4.hpp vec3.hpp vec2.hpp triangle_index.hpp
glutils.o: glutils.cpp glutils.hpp
mat4.o: mat4.cpp mat4.hpp
vec2.o: vec2.cpp vec2.hpp
vec3.o: vec3.cpp vec3.hpp
triangle_index.o: triangle_index.cpp triangle_index.hpp
vertex_opengl.o: vertex_opengl.cpp vertex_opengl.hpp vec2.hpp vec3.hpp
tga.o: tga.cpp image.hpp
mesh.o: mesh.cpp mesh.hpp mat4.hpp format/mesh_io_obj.hpp format/mesh_io_off.hpp
format/mesh_io_obj.o: format/mesh_io_obj.cpp format/mesh_io_obj.hpp
format/mesh_io_off.o: format/mesh_io_off.cpp format/mesh_io_off.hpp

.PHONY: clean
clean: 
	rm -f format/*.o *.o executable *~ 
