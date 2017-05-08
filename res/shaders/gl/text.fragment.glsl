#version 120

uniform sampler2D uTextureSampler0;
uniform vec4 uColor;

varying vec2 vTexCoord0;

void main(void)
{
  vec4 texColor = texture2D(uTextureSampler0, vTexCoord0);
  gl_FragColor = vec4(uColor.rgb, texColor.a);
}

