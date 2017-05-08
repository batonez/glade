#pragma once

extern double DOUBLE_CUBE_DIAGONAL;
extern float  RADIANS_IN_DEGREE;
extern float  PI;
extern float  TAU;

// parameter is the probability of returning true
bool  throw_coin(float probability = 0.5f);
float random(float from, float to);
float degrees_to_radians(float degrees);
float radians_to_degrees(float radians);
float simplify_angle_radians(float angle);
float smallerof(float first, float second);
float largerof(float first, float second);
float clamp(float value, float min, float max);
