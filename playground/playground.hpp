#ifndef PLAYGROUND_HPP
#define PLAYGROUND_HPP

#include <GL/glew.h>

#define FOURCC_DXT1 0x31545844  // Equivalent to "DXT1" in ASCII
#define FOURCC_DXT3 0x33545844  // Equivalent to "DXT3" in ASCII
#define FOURCC_DXT5 0x35545844  // Equivalent to "DXT5" in ASCII

// Vertices for a cube. Three consecutive floats give a 3D vertex; Three consecutive vertices give a
// triangle. A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3
// vertices.
const GLfloat g_vertex_buffer_data[] = {
    -1.0f, -1.0f, -1.0f,  // triangle 1 : begin
    -1.0f, -1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,  // triangle 1 : end
    1.0f, 1.0f, -1.0f,  // triangle 2 : begin
    -1.0f, -1.0f, -1.0f,
    -1.0f, 1.0f, -1.0f,  // triangle 2 : end
    1.0f, -1.0f, 1.0f,
    -1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f, 1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, -1.0f,
    1.0f, -1.0f, 1.0f,
    -1.0f, -1.0f, 1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f, -1.0f, 1.0f,
    1.0f, -1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f, 1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, -1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, -1.0f,
    -1.0f, 1.0f, -1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, -1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f, -1.0f, 1.0f};

// Two UV coordinatesfor each vertex. They were created with Blender. You'll learn shortly how to do this yourself.
const GLfloat g_uv_buffer_data[] = {
    0.000059f, /* 1.0f - */ 0.000004f,
    0.000103f, /* 1.0f - */ 0.336048f,
    0.335973f, /* 1.0f - */ 0.335903f,
    1.000023f, /* 1.0f - */ 0.000013f,
    0.667979f, /* 1.0f - */ 0.335851f,
    0.999958f, /* 1.0f - */ 0.336064f,
    0.667979f, /* 1.0f - */ 0.335851f,
    0.336024f, /* 1.0f - */ 0.671877f,
    0.667969f, /* 1.0f - */ 0.671889f,
    1.000023f, /* 1.0f - */ 0.000013f,
    0.668104f, /* 1.0f - */ 0.000013f,
    0.667979f, /* 1.0f - */ 0.335851f,
    0.000059f, /* 1.0f - */ 0.000004f,
    0.335973f, /* 1.0f - */ 0.335903f,
    0.336098f, /* 1.0f - */ 0.000071f,
    0.667979f, /* 1.0f - */ 0.335851f,
    0.335973f, /* 1.0f - */ 0.335903f,
    0.336024f, /* 1.0f - */ 0.671877f,
    1.000004f, /* 1.0f - */ 0.671847f,
    0.999958f, /* 1.0f - */ 0.336064f,
    0.667979f, /* 1.0f - */ 0.335851f,
    0.668104f, /* 1.0f - */ 0.000013f,
    0.335973f, /* 1.0f - */ 0.335903f,
    0.667979f, /* 1.0f - */ 0.335851f,
    0.335973f, /* 1.0f - */ 0.335903f,
    0.668104f, /* 1.0f - */ 0.000013f,
    0.336098f, /* 1.0f - */ 0.000071f,
    0.000103f, /* 1.0f - */ 0.336048f,
    0.000004f, /* 1.0f - */ 0.671870f,
    0.336024f, /* 1.0f - */ 0.671877f,
    0.000103f, /* 1.0f - */ 0.336048f,
    0.336024f, /* 1.0f - */ 0.671877f,
    0.335973f, /* 1.0f - */ 0.335903f,
    0.667969f, /* 1.0f - */ 0.671889f,
    1.000004f, /* 1.0f - */ 0.671847f,
    0.667979f, /* 1.0f - */ 0.335851f};

#endif