#version 120

#ifdef GL_ES
precision mediump float;
#endif

#extension GL_OES_standard_derivatives : enable

uniform float time;
uniform vec2 mouse;
uniform vec2 resolution;

float whichHalfPlane(vec2 position, vec2 one, vec2 two)
{
   float lineEquation = (one.y - two.y)*position.x + (two.x - one.x)*position.y + (one.x*one.y - two.x*two.y);
   return lineEquation;
}

void main( void ) {

vec2 position = ( gl_FragCoord.xy / resolution.xy ) * 2.0 - 1.0;


float beginX = 0.3;
float beginY = -0.2;
float width  = 0.1;

vec3 one   = vec3(beginX, beginY, 1.0);
vec3 two   = vec3(beginX+width, beginY, 1.0);
vec3 three = vec3(beginX+width, beginY+width, 1.0);
vec3 four  = vec3(beginX, beginY+width, 1.0);

one =   vec3(one.x/one.z, one.y/one.z, 1.0);
two   = vec3(two.x/two.z, two.y/two.z, 1.0);
three = vec3(three.x/three.z, three.y/three.z, 1.0);
four  = vec3(four.x/four.z, four.y/four.z, 1.0);

float color = 0.0;

if (whichHalfPlane(position, one.xy, two.xy) > 0.0 &&
    whichHalfPlane(position, two.xy, three.xy) < 0.0 &&
    whichHalfPlane(position, four.xy, three.xy) < 0.0 &&
    whichHalfPlane(position, one.xy, four.xy) > 0.0
   )
  color = 1.0;

gl_FragColor = vec4( vec3(color ), 1.0 );

}