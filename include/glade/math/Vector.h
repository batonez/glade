#pragma once

#include <cmath>

// FIXME: make overloaded operators pls

class Vector2i {
public:
  int x, y;
  
  Vector2i(void) { setZero(); }
  Vector2i(int x, int y) { this->x = x; this->y = y; }
  void setZero(void) { x = y = 0; }
  void set(const Vector2i &vector) { x  = vector.x; y  = vector.y; }
  void set(int x, int y) { this->x = x; this->y = y; }
  void add(const Vector2i &vector) { x += vector.x; y += vector.y; }
  void dot(const Vector2i &vector) { x *= vector.x; y *= vector.y; }
  void add(const Vector2i &vector, Vector2i &result) { result.x = x + vector.x; result.y = y + vector.y; }
  void dot(const Vector2i &vector, Vector2i &result) { result.x = x * vector.x; result.y = y * vector.y; }
};

class Vector3i {
public:
  int x, y, z;

  Vector3i(void) { setZero(); }
  Vector3i(int x, int y, int z) { this->x = x; this->y = y; this->z = z; }
  void setZero(void) { x = y = z = 0; }
  void set(const Vector3i &vector) { x  = vector.x; y  = vector.y; z  = vector.z; }
  void set(int x, int y, int z) { this->x = x; this->y = y; this->z = z; }
  void add(const Vector3i &vector) { x += vector.x; y += vector.y; z += vector.z; }
  void dot(const Vector3i &vector) { x *= vector.x; y *= vector.y; z *= vector.z; }
  void add(const Vector3i &vector, Vector3i &result) { result.x = x + vector.x; result.y = y + vector.y; result.z = z + vector.z; }
  void dot(const Vector3i &vector, Vector3i &result) { result.x = x * vector.x; result.y = y * vector.y; result.z = z * vector.z; }
};

class Vector2f {
public:
  float x, y;
  
  Vector2f(void) { setZero(); }
  Vector2f(float x, float y) { this->x = x; this->y = y; }
  void setZero(void) { x = y = 0.0f; }
  void set(const Vector2f &vector) { x  = vector.x; y  = vector.y; }
  void set(float x, float y) { this->x = x; this->y = y; }
  void add(const Vector2f &vector) { x += vector.x; y += vector.y; }
  void dot(const Vector2f &vector) { x *= vector.x; y *= vector.y; }
  void add(const Vector2f &vector, Vector2f &result) { result.x = x + vector.x; result.y = y + vector.y; }
  void dot(const Vector2f &vector, Vector2f &result) { result.x = x * vector.x; result.y = y * vector.y; }
};

class Vector3f {
public:
  float x, y, z;
  
  Vector3f(void) { setZero(); }
  Vector3f(float x, float y, float z) { this->x = x; this->y = y; this->z = z; }
  bool isZero() { return (x == 0.0f) && (y == 0.0f) && (z == 0.0f); }
  void setZero(void) { x = y = z = 0.0f; }
  void set(const Vector3f &vector) { x  = vector.x; y  = vector.y; z  = vector.z; }
  void set(float x, float y, float z) { this->x = x; this->y = y; this->z = z; }
  void add(const Vector3f &vector) { x += vector.x; y += vector.y; z += vector.z; }
  void subtract(const Vector3f &vector) { x -= vector.x; y -= vector.y; z -= vector.z; }
  void subtract(const Vector3f &vector, Vector3f &result) { result.x = x - vector.x; result.y = y - vector.y; result.z = z - vector.z; }
  void dot(const Vector3f &vector) { x *= vector.x; y *= vector.y; z *= vector.z; }
  void dot(float num) { x *= num; y *= num; z *= num; }
  void add(const Vector3f &vector, Vector3f &result) { result.x = x + vector.x; result.y = y + vector.y; result.z = z + vector.z; }
  void dot(const Vector3f &vector, Vector3f &result) { result.x = x * vector.x; result.y = y * vector.y; result.z = z * vector.z; }
  float magnitude() { return ::sqrt(x*x + y*y + z*z); }
  
  void cross(const Vector3f &vector, Vector3f &result)
  {
    result.x = y * vector.z - z * vector.y;
    result.y = z * vector.x - x * vector.z;
    result.z = x * vector.y - y * vector.x;
  }

  void normalize()
  {
    float mag = magnitude();

    if (mag != 0) {
      x /= mag;
      y /= mag;
      z /= mag;
    } else {
      x = y = z = 0.0f;
    }
  }
};

class Vector4f {
public:
  float x, y, z, w;
  
  Vector4f(void) { setZero(); }
  Vector4f(float x, float y, float z, float w) { this->x = x; this->y = y; this->z = z; this->w = w;}
  void setZero(void) { x = y = z = w = 0.0f; }
  void set(const Vector4f &vector) { x  = vector.x; y  = vector.y; z  = vector.z; w  = vector.w; }
  void set(float x, float y, float z, float w) { this->x = x; this->y = y; this->z = z; this->w = w; }
  void add(const Vector4f &vector) { x += vector.x; y += vector.y; z += vector.z; w += vector.w; }
  void dot(const Vector4f &vector) { x *= vector.x; y *= vector.y; z *= vector.z; w *= vector.w; }
  void add(const Vector4f &vector, Vector4f &result) { result.x = x + vector.x; result.y = y + vector.y; result.z = z + vector.z; result.w = w + vector.w; }
  void dot(const Vector4f &vector, Vector4f &result) { result.x = x * vector.x; result.y = y * vector.y; result.z = z * vector.z; result.w = w * vector.w; }
  float magnitude() { return ::sqrt(x*x + y*y + z*z + w*w); }

  void normalize()
  {
    float mag = magnitude();

    if (mag != 0) {
      x /= mag;
      y /= mag;
      z /= mag;
      w /= mag;
    } else {
      x = y = z = w = 0.0f;
    }
  }
};

class Vector4i {
public:
  int x, y, z, w;
  
  Vector4i(void) { setZero(); }
  Vector4i(int x, int y, int z, int w) { this->x = x; this->y = y; this->z = z; this->w = w;}
  void setZero(void) { x = y = z = w = 0.0f; }
  void set(const Vector4i &vector) { x  = vector.x; y  = vector.y; z  = vector.z; w  = vector.w; }
  void set(int x, int y, int z, int w) { this->x = x; this->y = y; this->z = z; this->w = w; }
  void add(const Vector4i &vector) { x += vector.x; y += vector.y; z += vector.z; w += vector.w; }
  void dot(const Vector4i &vector) { x *= vector.x; y *= vector.y; z *= vector.z; w *= vector.w; }
  void add(const Vector4i &vector, Vector4i &result) { result.x = x + vector.x; result.y = y + vector.y; result.z = z + vector.z; result.w = w + vector.w; }
  void dot(const Vector4i &vector, Vector4i &result) { result.x = x * vector.x; result.y = y * vector.y; result.z = z * vector.z; result.w = w * vector.w; }
};

