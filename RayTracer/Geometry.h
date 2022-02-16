#pragma once

#include <vector>
#include <iostream>

class Vec3f {

public:
	
	Vec3f(const float x1, const float y1, const float z1) :
		p{x1, y1, z1} {}
	Vec3f():
		p{ 0 } {}
	
	~Vec3f();	

	// Call x, y, and z value

	float x() const { return p[0]; };
	float y() const { return p[1]; };
	float z() const { return p[2]; };


	// Overloaded functions

	Vec3f& operator*=(const Vec3f& ref);
	Vec3f& operator*=(const float t);

	Vec3f& operator+=(const Vec3f& ref);
	Vec3f& operator-=(const Vec3f& ref);
	Vec3f& operator/=(float t);

	Vec3f& operator=(const Vec3f& ref);
	bool operator<=(const Vec3f& ref);

	Vec3f operator-(const Vec3f& ref) const;
	Vec3f operator/(float t) const;
	Vec3f operator*(float t) const;
	Vec3f operator+(const Vec3f& ref) const;


	float& operator[](unsigned int i);

	// Scalar Product
	//int Dot(const Vec3f& v1, const Vec3f& v2);

	// Unit vector calculation
	Vec3f& unitVec();

	//Norm
	float length();

	// Logger
	void Log();



private:
	float p[3];


};
