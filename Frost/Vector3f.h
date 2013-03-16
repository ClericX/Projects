#pragma once

#include "DLL.h"

#include <cmath>

class FROSTAPI Vector3f
{
public:
	float x;
	float y;
	float z;

	Vector3f(void);
	Vector3f(float x_, float y_, float z_);
	void Set(float x_, float y_, float z_);
	float length(void);
	void normalize(void);

	// Static utility methods
	static float distance(const Vector3f &v1, const Vector3f &v2);
	static float dotProduct(const Vector3f &v1,  const Vector3f &v2 );
	static Vector3f crossProduct(const Vector3f &v1, const Vector3f &v2);

	// Operators...
	Vector3f operator + (const Vector3f &other);
	Vector3f operator - (const Vector3f &other);
	Vector3f operator * (const Vector3f &other);
	Vector3f operator / (const Vector3f &other);

	Vector3f operator * (const float scalar);
	friend Vector3f operator * (const float scalar, const Vector3f &other);
	
	Vector3f& operator = (const Vector3f &other);
	Vector3f& operator += (const Vector3f &other);
	Vector3f& operator -= (const Vector3f &other);

	Vector3f operator + (void) const;
	Vector3f operator - (void) const;

	bool operator == (Vector3f &other);
	bool operator != (Vector3f &other);

	static Vector3f ZERO;
};