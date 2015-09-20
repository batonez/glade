#include <math.h>
#include <stdlib.h>

double DOUBLE_CUBE_DIAGONAL = 1.4422495703074083823216383107801;

bool throw_coin(float probability)
{
  return (::rand() % 100) < ::round(probability * 100);
}
