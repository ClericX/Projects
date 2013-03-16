#pragma once

#include "DLL.h"
#include "Vector3f.h"

#define DEGTORAD(degree) ((degree) * (3.141592654f / 180.0f))
#define RADTODEG(radian) ((radian) * (180.0f / 3.141592654f))

class FROSTAPI Matrix4x4f
{
public:

    float m[16];

    Matrix4x4f()  { Identity(); }

    Matrix4x4f( float m0, float m4, float  m8, float m12,
               float m1, float m5, float  m9, float m13,
               float m2, float m6, float m10, float m14,
               float m3, float m7, float m11, float m15 );

    void Identity(void);
    void translate(const Vector3f &trans);
    void translate_x(const float &dist);
    void translate_y(const float &dist);
    void translate_z(const float &dist);
    void rotate(const float &angle, Vector3f &axis);
    void rotate_x(const float &angle);
    void rotate_y(const float &angle);
    void rotate_z(const float &angle);
    void scale(const Vector3f &scale);
    void transformPoint( Vector3f *vec );
    void transformVector( Vector3f *vec );

    // Operators...
    Matrix4x4f operator + (const Matrix4x4f &other);
    Matrix4x4f operator - (const Matrix4x4f &other);
    Matrix4x4f operator * (const Matrix4x4f &other);

    Matrix4x4f operator * (const float scalar);
};