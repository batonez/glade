#version 120

#ifdef GL_ES
precision mediump float;
#endif

#extension GL_OES_standard_derivatives : enable  

uniform float time;
uniform vec2 mouse;
uniform vec2 resolution;

void main( void ) {
  float aspect = resolution.x / resolution.y;
  vec2 fragPos = gl_FragCoord.xy / resolution.xy;
  float posModifierY = 0.05*sin(time + fragPos.x*39.0);
  float posModifierX = 0.05*sin(time + fragPos.y*39.0);
  fragPos.y += posModifierY;
  fragPos.x += posModifierX;
	
  vec2 circleCenter = vec2(0.5, 0.5);
  float radius = 0.3;
  vec3 resultColor = vec3(fragPos.xy, 1.0);
	
  float diffX = max(fragPos.x, circleCenter.x) - min(fragPos.x, circleCenter.x);
  float diffY = max(fragPos.y, circleCenter.y) - min(fragPos.y, circleCenter.y);
  float distToCenter = sqrt(diffX*diffX*aspect*aspect + diffY*diffY);
  
  resultColor = vec3(smoothstep(radius + fwidth(distToCenter), radius, distToCenter));
  
  if (distToCenter < radius)
  resultColor = vec3(resultColor.x * fragPos.x * sin(time*3.), resultColor.y * fragPos.y * cos(time*3.0), 1.0);
  
  gl_FragColor = vec4( resultColor.xyz, 1.0 );

}