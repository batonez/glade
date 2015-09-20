precision mediump float;

uniform sampler2D uTextureSampler0;

varying vec2 vTexCoord0;
varying vec4 vColor;

vec4 blendColors(vec4 baseColor, vec4 overlayColor) {
  return vec4(
    baseColor.r * (1.0 - overlayColor.a) + overlayColor.r * overlayColor.a,
    baseColor.g * (1.0 - overlayColor.a) + overlayColor.g * overlayColor.a,
    baseColor.b * (1.0 - overlayColor.a) + overlayColor.b * overlayColor.a,
    baseColor.a * (1.0 - overlayColor.a) + overlayColor.a * overlayColor.a);
}

void main(void)
{
  vec4 texColor = texture2D(uTextureSampler0, vTexCoord0);
  gl_FragColor = blendColors(texColor, vColor);
}

