#version 330 core

// Declares input data
// “layout(location = 0)” refers to the buffer we use to feed the
// vertexPosition_modelspace attribute
layout(location = 0) in vec3 vertexPosition_modelspace;

// Values that stay constant for the whole mesh.
uniform mat4 MVP;

void main() {
  // Output position of the vertex, in clip space : MVP * position
  gl_Position = MVP * vec4(vertexPosition_modelspace, 1);
}
