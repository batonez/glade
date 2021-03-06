const float ZERO = 0.0, ONE = 1.0;

uniform mat4    uProjectionMatrix;
uniform mat4    uWorldViewMatrix;

uniform vec3    uLightDirection;
uniform vec3    uLightHalfplane;
uniform vec4    uLightAmbient;
uniform vec4    uLightDiffuse;
uniform vec4    uLightSpecular;
uniform vec4    uMaterialAmbient;
uniform vec4    uMaterialDiffuse;
uniform vec4    uMaterialSpecular;
uniform float   uMaterialShininess;

attribute vec4  aPosition;
attribute vec3  aNormal;
attribute vec2  aTexCoord0;

varying vec4    vColor;

vec4 calculateLight(vec3 normal) {
  vec4 computedColor = vec4(ZERO, ZERO, ZERO, ZERO);
  float ndotl;
  float ndoth;
  ndotl = max(ZERO, dot(normal, uLightDirection));
  ndoth = max(ZERO, dot(normal, uLightHalfplane));
  computedColor += (uLightAmbient * uMaterialAmbient);
  computedColor += (ndotl * uLightDiffuse * uMaterialDiffuse);

  if (ndoth > ZERO) {
    computedColor += (pow(ndoth, uMaterialShininess) *
    uMaterialSpecular *
    uLightSpecular);
  }

  return computedColor;
}

void main(void) {
  gl_Position = uProjectionMatrix * uWorldViewMatrix * aPosition;

  vec4 rotatedNormal = normalize(uWorldViewMatrix * vec4(aNormal.xyz, ZERO));
  vColor = calculateLight(rotatedNormal.xyz);
}

