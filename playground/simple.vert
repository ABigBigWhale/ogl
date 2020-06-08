#version 330 core

// Input data
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec2 vertexUV;

out vec2 uv;

// Values that stay constant for the whole mesh.
uniform mat4 mvp;

void main() {
  // Output position of the vertex, in clip space : MVP * position
  gl_Position = mvp * vec4(vertexPosition_modelspace, 1);

  // The color of each vertex will be interpolated
  // to produce the color of each fragment
  uv = vertexUV;
}
