#version 330 core

// Interpolated values from the vertex shaders
in vec2 uv;
// in vec3 fragmentColor;

out vec3 color;

uniform sampler2D textureSampler;

void main() {
  // color of the texture at the specified UV
  color = texture(textureSampler, uv).rgb;
}
