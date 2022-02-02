#pragma once

#include "Geometry.h"

class Lights {

public:
	Lights(const Vec3f& p, float i) :
		pos(p), intensity(i) {}


public:
	Vec3f pos{};
	float intensity{};



};