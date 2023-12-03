#version 300 es

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;
uniform mat3 normalMatrix;

// sliders
// Light properties
uniform vec4 lightDirWorldSpace;

out vec3 fragV;
out vec3 fragL;
out vec3 fragN;

float dist_from_planet_center(vec3 pos) {
  float sqrsum = pow(pos.x, 2.0) + pow(pos.y, 2.0) + pow(pos.z, 2.0);
  return sqrt(sqrsum);
}

void main() {
  /* old code
  vec4 posEyeSpace = viewMatrix * modelMatrix * vec4(inPosition, 1);

  float i = 1.0 - (-posEyeSpace.z / 5.0);
  if (dist_from_planet_center(inPosition) > 1.009) {
    fragColor = vec4(i, i, i, 1) * vec4(0.0f, 1.0f, 0.0f, 1.0f);
  } else {
    fragColor = vec4(i, i, i, 1) * vec4(0.0f, 0.0f, 1.0f, 1.0f);
  }

  gl_Position = projMatrix * posEyeSpace;
  */

  vec3 P = (viewMatrix * modelMatrix * vec4(inPosition, 1.0)).xyz;
  vec3 N = normalMatrix * inNormal;
  vec3 L = -(viewMatrix * lightDirWorldSpace).xyz;

  fragL = L;
  fragV = -P;
  fragN = N;

  gl_Position = projMatrix * vec4(P, 1.0);
}