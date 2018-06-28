#pragma once
#include "Vector3.h"

class Scan_Point 
{
public:
	float x, y, z, I;
	bool RGB;
	short R, G, B;

public:
	inline Scan_Point() {}

	inline Scan_Point(float x, float y, float z, float I, short R, short G, short B)
		: x(x), y(y), z(z), I(I), RGB(1), R(R), G(G), B(B) {}	

	inline Scan_Point(float x, float y, float z, float I)
		: x(x), y(y), z(z), I(I), RGB(0), R(0), G(0), B(0) {}

	/*inline Scan_Point(double x, double y, double z, int I, short R, short G, short B)
		: x(x), y(y), z(z), I(I), RGB(1), R(R), G(G), B(B) {}

	inline Scan_Point(double x, double y, double z, int I)
		: x(x), y(y), z(z), I(I), RGB(0), R(0), G(0), B(0) {}*/

// ---------------------------------------------------------------------------------------------------- //

	inline bool operator==(const Scan_Point &p) const
	{
		return x == p.x && y == p.y && z == p.z;
	}

	//inline bool operator!=(const Point3 &p) const
	//{
	//	return x != p.x || y != p.y || z != p.z;
	//}
	//
	//inline Point3 operator+(const Vector3 v) const
	//{
	//	return Point3(x+v.x, y+v.y, z+v.z);
	//}
	//
	//inline Point3 & operator+=(const Vector3 &v)
	//{
	//	x += v.x;
	//	y += v.y;
	//	z += v.z;
	//	return *this;
	//}
	//
	//inline Point3 operator-(const Vector3 &v) const
	//{
	//	return Point3(x-v.x, y-v.y, z-v.z);
	//}
	//
	//inline Point3 & operator-=(const Vector3 &v)
	//{
	//	x -= v.x;
	//	y -= v.y;
	//	z -= v.z;
	//	return *this;
	//}
	//
	//inline Point3 operator-() const
	//{
	//	return Point3(-x, -y, -z);
	//}

};

// ---------------------------------------------------------------------------------------------------- //

// Writes Scan_Point p to output stream os.
inline std::ostream & operator<<(std::ostream &os, const Scan_Point &p)
{
	//os << p.x << " " << p.y << " " << p.z << " " << p.I << " " << p.RGB << " " << p.R << " " << p.G << " " << p.B;
	os << p.x << " " << p.y << " " << p.z << " " << p.I << " " << p.R << " " << p.G << " " << p.B;
	return os;
}