#include <glade/math/Vector.h>
#include <glade/render/Perception.h>

Perception::Perception():
  baseLightDirection(-1.0f, 0.0f,  1.0f),
  baseLookVector( 0.0f, 0.0f, -1.0f),
  lightDirection(baseLightDirection),
  lookVector(baseLookVector)
{
  baseLightDirection.normalize();
  lightDirection.set(baseLightDirection);

  baseLookVector.normalize();
  lookVector.set(baseLookVector);

  lightDirection.subtract(baseLookVector, halfplane);
  halfplane.normalize();

  setUniform("uLightAmbient",   Vector4f(0.5f, 0.5f, 0.5f, 1.0f));
  setUniform("uLightDiffuse",   Vector4f(0.5f, 0.5f, 0.5f, 1.0f));
  setUniform("uLightSpecular",  Vector4f(1.0f, 1.0f, 1.0f, 1.0f));

  setUniform("uLightDirection", lightDirection);
  setUniform("uLightHalfplane", halfplane);
}

void Perception::adjust()
{
  // rotating the light direction vector
  Vector4f rotatedVector;
  Vector4f vector(baseLightDirection.x, baseLightDirection.y, baseLightDirection.z, 0.0f);

  float matrix[16];
  getTransform()->getCameraMatrix(matrix);
  Matrix::multiplyMV(rotatedVector, matrix, vector);

  lightDirection.set(rotatedVector.x, rotatedVector.y, rotatedVector.z);
  lightDirection.normalize();

  // rotating the look vector
  getTransform()->getMatrix(matrix);
  vector.set(baseLookVector.x, baseLookVector.y, baseLookVector.z, 0.0f);
  Matrix::multiplyMV(rotatedVector, matrix, vector);
  lookVector.set(rotatedVector.x, rotatedVector.y, rotatedVector.z);
  lookVector.normalize();

  // recalculating half plane vector
  halfplane.set(lightDirection);
  halfplane.subtract(lookVector);
  halfplane.normalize();

  setUniform("uLightDirection", lightDirection);
  setUniform("uLightHalfplane", halfplane);
}
