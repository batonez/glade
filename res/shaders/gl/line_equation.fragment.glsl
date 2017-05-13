#ifdef GL_ES
precision mediump float;
#endif

#extension GL_OES_standard_derivatives : enable

uniform float time;
uniform vec2 mouse;
uniform vec2 resolution;

void main( void ) {

vec2 position = ( gl_FragCoord.xy / resolution.xy ) * 2.0 - 1.0;


vec3 one   = vec3(0.0, 0.0, 1.0);
vec3 two   = vec3(0.1, 0.1, 1.0);

float lineEquation = (one.y - two.y)*position.x + (two.x - one.x)*position.y + (one.x*one.y - two.x*two.y);

float color = 0.0;

if (lineEquation < 0.0)
  color = 1.0;

gl_FragColor = vec4( vec3(color ), 1.0 );

}