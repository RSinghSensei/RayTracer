#pragma once

#include "Geometry.h"

class Sphere {
public:
	Sphere(const Vec3f& center, float radius, const Vec3f& material, const Vec3f& albedo, const float&specular) :
		s_Center(center), s_Radius(radius), s_Material(material), specularExponent(specular)  {}
	Sphere() :
		s_Radius{} {}	


public:
	Vec3f s_Center{};
	float s_Radius{};
	Vec3f s_Material{};
	Vec3f albedo{};
	float specularExponent{};

};