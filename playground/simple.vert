#version 330 core

// Declares input data
// “layout(location = 0)” refers to the buffer we use to feed the
// vertexPosition_modelspace attribute
layout(location = 0) in vec3 vertexPosition_modelspace;

void main() {
  gl_Position.xyz = vertexPosition_modelspace;
  gl_Position.w = 1.0;
}