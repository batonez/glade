uniform sampler2D uTextureSampler0;

varying vec2 vTexCoord0;

void main(void)
{
  vec4 texColor = texture2D(uTextureSampler0, vTexCoord0);
  gl_FragColor = texColor;
}

