#include "Geometry.h"

Vec3f::~Vec3f(){}

Vec3f& Vec3f::operator*=(const float t) {
	p[0] *= t;
	p[1] *= t;
	p[2] *= t;	
	
	return *this;
}


Vec3f& Vec3f::operator*=(const Vec3f &ref) {
	p[0] *= ref.p[0];
	p[1] *= ref.p[1];
	p[2] *= ref.p[2];

	return *this;
}


Vec3f& Vec3f::operator+=(const Vec3f& ref) {
	p[0] += ref.p[0];
	p[1] += ref.p[1];
	p[2] += ref.p[2];

	return *this;
}

Vec3f& Vec3f::operator-=(const Vec3f& ref) {
	p[0] -= ref.p[0];
	p[1] -= ref.p[1];
	p[2] -= ref.p[2];

	return *this;
}

Vec3f& Vec3f::operator/=(float t){
	*this*= (1 / t);
	return *this;
}

Vec3f& Vec3f::operator=(const Vec3f& ref) {
	p[0] = ref.p[0];
	p[1] = ref.p[1];
	p[2] = ref.p[2];

	return *this;
}

bool Vec3f::operator<=(const Vec3f& ref) {
	if (p[0] <= ref.p[0] && p[1] <= ref.p[1] && p[2] <= ref.p[2]) { return true; }
	return false;
}

Vec3f Vec3f::operator-(const Vec3f& ref) const {
	Vec3f ret;
	ret[0] = p[0] - ref.p[0];
	ret[1] = p[1] - ref.p[1];
	ret[2] = p[2] - ref.p[2];

	return ret;
}

Vec3f Vec3f::operator/(float t) const {
	Vec3f ret;
	ret[0] = p[0] * (1/t);
	ret[1] = p[1] * (1/t);
	ret[2] = p[2] * (1/t);

	return ret;
}

Vec3f Vec3f::operator*(float t) const {
	Vec3f ret;
	ret[0] = p[0] * t;
	ret[1] = p[1] * t;
	ret[2] = p[2] * t;
	return ret;

}

Vec3f Vec3f::operator+(const Vec3f &ref) const {
	Vec3f ret;
	ret[0] = p[0] + ref.p[0];
	ret[1] = p[1] + ref.p[1];
	ret[2] = p[2] + ref.p[2];
	return ret;

}

float& Vec3f::operator[](unsigned int i) {
	return p[i];
}
//
//int Vec3f::Dot(const Vec3f& v1, const Vec3f& v2) {
//	return v1.x() * v2.x() + v1.y() + v2.y() + v1.z() + v2.z();
//}

void Vec3f::Log() {
	std::cout << p[0] << " " << p[1] << " " << p[2] << std::endl;
}

Vec3f& Vec3f::unitVec() {
	float lengthS = p[0] * p[0] + p[1] * p[1] + p[2] * p[2];
	float length = sqrt(lengthS);
	*this /= length;
	return *this;


}