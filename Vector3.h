#pragma once

#include <math.h>
#include <string>

#define _USE_MATH_DEFINES

class Vector3
{

friend class Point3;

public:
	double x, y, z;

// ---------------------------------------------------------------------------------------------------- //

public:
	// Constructs a vector without explicit initialization.
	inline Vector3() {}

	// Constructs a vector with coordinates (x, y).
	inline Vector3(const double x, const double y, const double z) 
		: x(x), y(y), z(z) {}

	// Constructs a copy of vector v.
	inline Vector3(const Vector3 &v) 
		: x(v.x), y(v.y), z(v.z) {}

// ---------------------------------------------------------------------------------------------------- //

	// Returns true if this vector equals vector v.
	inline bool operator==(const Vector3 &v) const
	{
		const double EPSILON = 0.000000000000001;
		return abs(x - v.x) < EPSILON && abs(y - v.y) < EPSILON && abs(z - v.z) < EPSILON;
	}

	// Returns false if this vector equals vector v.
	inline bool operator!=(const Vector3 &v) const
	{
		const double EPSILON = 0.000000000000001;
		return abs(x - v.x) > EPSILON || abs(y - v.y) > EPSILON || abs(z - v.z) > EPSILON;
	}

	//! Returns true if this vector comes before vector v in a non-ambiguous sorting order.
	inline bool operator<(const Vector3 &v) const
	{
		if (x < v.x)
			return true;
		else if (x > v.x)
			return false;
		else if (y < v.y)
			return true;
		else if (y > v.y)
			return false;
		else
			return z < v.z;
	}

	// Returns the result vector of the addition of this vector and vector v.
	inline Vector3 operator+(const Vector3 &v) const
	{
		return Vector3(x+v.x, y+v.y, z+v.z);
	}

	// Adds vector v to this vector.
	inline Vector3 & operator+=(const Vector3 &v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}

	// Returns the result vector of the subtraction of vector v from this vector.
	inline Vector3 operator-(const Vector3 &v) const
	{
		return Vector3(x-v.x, y-v.y, z-v.z);
	}

	// Subtracts vector v from this vector.
	inline Vector3 & operator-=(const Vector3 &v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return *this;
	}

	// Returns the result vector of the multiplication of this vector with scalar s.
	inline Vector3 operator*(const double &s) const
	{
		return Vector3(x*s, y*s, z*s);
	}

	// Multiplies this vector with scalar s.
	inline Vector3 & operator*=(const double s)
	{
		x *= s;
		y *= s;
		z *= s;
		return *this;
	}

	// Returns the result vector of the division of this vector by scalar s.
	inline Vector3 operator/(const double s) const
	{
		double f = 1.0 / s;
		return Vector3(x*f, y*f, z*f);
	}

	// Divides this vector by scalar s.
	inline Vector3 & operator/=(const double s)
	{
		double f = 1.0 / s;
		x *= f;
		y *= f;
		z *= f;
		return *this;
	}

	// Returns the negative vector of this vector.
	inline Vector3 operator-() const
	{
		return Vector3(-x, -y, -z);
	}

// ---------------------------------------------------------------------------------------------------- //

	// Returns the length of this vector.
	inline double Length() const
	{
		return sqrt(x*x + y*y + z*z);
	}

};

// ---------------------------------------------------------------------------------------------------- //

// Writes vector v to output stream os.
inline std::ostream & operator<<(std::ostream &os, const Vector3 &v)
{
	os << v.x << " " << v.y << " " << v.z;
	return os;
}