#include <math.h>
#include <stdlib.h>

double DOUBLE_CUBE_DIAGONAL = 1.4422495703074083823216383107801;
float  RADIANS_IN_DEGREE = 0.0174533f;

bool throw_coin(float probability)
{
  return (::rand() % 100) < ::round(probability * 100);
}

float degrees_to_radians(float degrees)
{
  return degrees * RADIANS_IN_DEGREE;
}