#pragma once

#include <cmath>

#include "Vector.h"
#include "Matrix.h"

class Transform {
private:
	Vector3f position;
	Vector3f rotation;
	Vector3f scale;
	char invertor;
	
public:
	static float constrainAngle(float angle) { return angle < 360.0f ? angle : fmod(angle, 360.0f); }
	Transform(void) : Transform(0, 0, 0, 0, 0, 0, 1, 1, 1) {}
	
	Transform(float posX, float posY, float posZ, float rotX, float rotY, float rotZ, float scaleX, float scaleY, float scaleZ) :
  invertor(1)
  {
		position.set(posX, posY, posZ);
		rotation.set(rotX, rotY, rotZ);
		scale.set(scaleX, scaleY, scaleZ);
	}
	
	Vector3f* getScaleP   (void) { return &scale; 	 }
	Vector3f* getPositionP(void) { return &position; }
	Vector3f* getRotationP(void) { return &rotation; }
	
	Vector3f  getScale   (void) const { return scale; 	 }
	Vector3f  getPosition(void) const { return position; }
	Vector3f  getRotation(void) const { return rotation; }
	
	void setPosition(float x, float y, float z) { position.set(x, y, z); }
	void setRotation(float x, float y, float z) { rotation.set(x, y, z); }
	void setScale	(float x, float y, float z) { scale.set	  (x, y, z); }
	
	void setPosition(const Vector3f &position) { this->position.set (position); }
	void setRotation(const Vector3f &rotation) { this->rotation.set (rotation); }
	void setScale	(const Vector3f &scale   ) { this->scale.set	(scale   ); }
	
  void set(Transform* transform) {
    setPosition(transform->getPosition());
    setRotation(transform->getRotation());
    setScale(transform->getScale());
  }
  
	void invertTranslation(bool invert) { invertor = invert ? -1 : 1; }
	bool isInvertedTranslation(void) { return invertor < 0; }
		
	void combineWith(const Transform &relativeTransform, Transform &resultTransform) {
		return Transform::combine(*this, relativeTransform, false, false, false, resultTransform);
	}
	
	void combineWith(const Transform &relativeTransform, bool preservePosition, bool preserveRotation, bool preserveScale, Transform &resultTransform) {
		return Transform::combine(*this, relativeTransform, preservePosition, preserveRotation, preserveScale, resultTransform);
	}
	
	/**
	 * Комбинирует два трансформа так, что их переносы и повороты складываются, а скейлы перемножаются
	 */
	static void combine(const Transform &baseTransform, const Transform &relativeTransform, bool preservePosition, bool preserveRotation, bool preserveScale, Transform &resultTransform) {
		if (preservePosition) {
			resultTransform.setPosition(relativeTransform.getPosition());
		} else {
			baseTransform.getPosition().add(relativeTransform.getPosition(), *resultTransform.getPositionP());
		}
		
		if (preserveRotation) {
			resultTransform.setRotation(relativeTransform.getRotation());
		} else {
			baseTransform.getRotation().add(relativeTransform.getRotation(), *resultTransform.getRotationP());
		}
		
		if (preserveScale) {
			resultTransform.setScale(relativeTransform.getScale());
		} else {
			baseTransform.getScale().dot(relativeTransform.getScale(), *resultTransform.getScaleP());
		}
	}
	
	/**
	 * Возвращает матрицу трансформации в пространстве.
	 * Поворот вокруг оси Z пока не поддерживается из-за gimbal lock
	 */
	void getMatrix(float* result) {
		Matrix::setIdentityM(result, 0);
		Matrix::translateM(result, 0, invertor * position.x, invertor * position.y, invertor * position.z);
		Matrix::rotateM(result, 0, rotation.x, 1, 0, 0);
		Matrix::rotateM(result, 0, rotation.y, 0, 1, 0);
		Matrix::scaleM(result, 0, scale.x, scale.y, scale.z);
	}
};
