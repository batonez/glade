const float ZERO = 0.0, ONE = 1.0;

uniform mat4 uProjectionMatrix;
uniform mat4 uWorldViewMatrix;

uniform float uTexCoordScaleX0;
uniform float uTexCoordScaleY0;
uniform float uTexCoordOffsetX0;
uniform float uTexCoordOffsetY0;

attribute vec4 aPosition;
attribute vec3 aNormal;
attribute vec2 aTexCoord0;

varying vec2 vTexCoord0;

vec4 transformTexCoords(vec2 texCoord, vec2 offset, vec2 scale) {
  vec4 result;
  result.y = 0;
  result.z = 0;

  if (scale.x < ZERO) {
    result.x = ONE + texCoord.x * scale.x - offset.x;
  } else {
    result.x = texCoord.x * scale.x + offset.x;
  }

  if (scale.y < ZERO) {
    result.y = ONE + texCoord.y * scale.y - offset.y;
  } else {
    result.y = texCoord.y * scale.y + offset.y;
  }

  return result;
}

void main() {
  gl_Position = uProjectionMatrix * uWorldViewMatrix * aPosition;
  vTexCoord0 = transformTexCoords(aTexCoord0, vec2(uTexCoordOffsetX0, uTexCoordOffsetY0), vec2(uTexCoordScaleX0, uTexCoordScaleY0));
}

