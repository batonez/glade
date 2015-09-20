const float ZERO = 0.0, ONE = 1.0;

uniform mat4 uProjectionMatrix;
uniform mat4 uWorldViewMatrix;

uniform float uTexCoordScaleX0;
uniform float uTexCoordScaleY0;
uniform float uTexCoordOffsetX0;
uniform float uTexCoordOffsetY0;

uniform int     uLight;
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

varying vec2    vTexCoord0;
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
  
  if (uLight != ZERO) {
     vec4 rotatedNormal = normalize(uWorldViewMatrix * vec4(aNormal.xyz, ZERO));
     vColor = calculateLight(rotatedNormal.xyz);
  } else {
     vColor = vec4(ZERO, ZERO, ZERO, ZERO);
  }
};
