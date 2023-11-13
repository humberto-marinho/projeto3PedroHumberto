#version 300 es

layout(location = 0) in vec3 inPosition;

//uniform vec4 color;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;

out vec4 fragColor;

float dist_from_planet_center(vec3 pos) {
  float sqrsum = pow(pos.x, 2.0) + pow(pos.y, 2.0) + pow(pos.z, 2.0);
  return sqrt(sqrsum);
}

void main() {
  vec4 posEyeSpace = viewMatrix * modelMatrix * vec4(inPosition, 1);

  float i = 1.0 - (-posEyeSpace.z / 5.0);
  if (dist_from_planet_center(inPosition) > 1.009) {
    fragColor = vec4(i, i, i, 1) * vec4(0.0f, 1.0f, 0.0f, 1.0f);
  } else {
    fragColor = vec4(i, i, i, 1) * vec4(0.0f, 0.0f, 1.0f, 1.0f);
  }

  gl_Position = projMatrix * posEyeSpace;
}