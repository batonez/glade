#pragma once

#include <math.h>
#include <string.h>
#include "Vector.h"

// Index of the matrix element
#define  I(_i, _j) ((_j)+ 4*(_i))
#define PI 3.141592653589793

class Matrix {
public:
  static void mx4transform(float x, float y, float z, float w, const float* pM, float* pDest) {
    pDest[0] = pM[0 + 4 * 0] * x + pM[0 + 4 * 1] * y + pM[0 + 4 * 2] * z + pM[0 + 4 * 3] * w;
    pDest[1] = pM[1 + 4 * 0] * x + pM[1 + 4 * 1] * y + pM[1 + 4 * 2] * z + pM[1 + 4 * 3] * w;
    pDest[2] = pM[2 + 4 * 0] * x + pM[2 + 4 * 1] * y + pM[2 + 4 * 2] * z + pM[2 + 4 * 3] * w;
    pDest[3] = pM[3 + 4 * 0] * x + pM[3 + 4 * 1] * y + pM[3 + 4 * 2] * z + pM[3 + 4 * 3] * w;
  }

  // FIXME: add offsets (just like in the multiplyMM)
  static void multiplyMV(float* r, const float* lhs, const float* rhs) {
    mx4transform(rhs[0], rhs[1], rhs[2], rhs[3], lhs, r);
  }
  
  static void multiplyMV(Vector4f &result, const float* lhs, const Vector4f &rhs) {
    float dest[4] = {0};
    mx4transform(rhs.x, rhs.y, rhs.z, rhs.w, lhs, dest);
    result.x = dest[0];
    result.y = dest[1];
    result.z = dest[2];
    result.w = dest[3];
  }
  
  static float length(float x, float y, float z) {
        return (float) sqrt(x * x + y * y + z * z);
    }
  
  static void setIdentityM(float sm[16], int smOffset) {
        for (int i=0 ; i<16 ; i++) {
            sm[smOffset + i] = 0.0f;
        }
    
        for(int i = 0; i < 16; i += 5) {
            sm[smOffset + i] = 1.0f;
        }
    }

    static void scaleM(float sm[16], int smOffset, const float m[16], int mOffset, float x, float y, float z) {
        for (int i=0 ; i<4 ; i++) {
            int smi = smOffset + i;
            int mi = mOffset + i;
            sm[     smi] = m[     mi] * x;
            sm[ 4 + smi] = m[ 4 + mi] * y;
            sm[ 8 + smi] = m[ 8 + mi] * z;
            sm[12 + smi] = m[12 + mi];
        }
    }

    static void scaleM(float m[16], int mOffset, float x, float y, float z) {
        for (int i=0 ; i<4 ; i++) {
            int mi = mOffset + i;
            m[     mi] *= x;
            m[ 4 + mi] *= y;
            m[ 8 + mi] *= z;
        }
    }

    static void translateM(float tm[16], int tmOffset, const float m[16], int mOffset, float x, float y, float z) {
        for (int i=0 ; i<12 ; i++) {
            tm[tmOffset + i] = m[mOffset + i];
        }
    
        for (int i=0 ; i<4 ; i++) {
            int tmi = tmOffset + i;
            int mi = mOffset + i;
            tm[12 + tmi] = m[mi] * x + m[4 + mi] * y + m[8 + mi] * z + m[12 + mi];
        }
    }

    static void translateM(float m[16], int mOffset, float x, float y, float z) {
        for (int i=0 ; i<4 ; i++) {
            int mi = mOffset + i;
            m[12 + mi] += m[mi] * x + m[4 + mi] * y + m[8 + mi] * z;
        }
    }

    static void rotateM(float rm[16], int rmOffset, const float m[16], int mOffset, float a, float x, float y, float z) {
        float r[16];
        setRotateM(r, 0, a, x, y, z);
        multiplyMM(rm, rmOffset, m, mOffset, r, 0);
    }

    static void rotateM(float m[16], int mOffset, float a, float x, float y, float z) {
        float temp[32];
        setRotateM(temp, 0, a, x, y, z);
        multiplyMM(temp, 16, m, mOffset, temp, 0);
        memcpy(m + mOffset, temp + 16, 16 * sizeof(float)); // было System.arraycopy(temp, 16, m, mOffset, 16);
    }
  
  static void multiplyMM(float* r, int rOffset, const float* lhs, int lhsOffset, const float* rhs, int rhsOffset);
    static void transposeM(float mTrans[16], int mTransOffset, const float m[16], int mOffset);
  static bool invertM(float mInv[16], int mInvOffset, const float m[16], int mOffset);
    static void orthoM(float m[16], int mOffset, float left, float right, float bottom, float top, float near, float far);
  static void frustumM(float m[16], int offset, float left, float right, float bottom, float top, float near, float far);
    static void perspectiveM(float m[16], int offset, float fovy, float aspect, float zNear, float zFar);
    static void setRotateM(float rm[16], int rmOffset, float a, float x, float y, float z);
    static void setRotateEulerM(float rm[16], int rmOffset, float x, float y, float z);
};