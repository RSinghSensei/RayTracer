#pragma once
#include <iostream>
#include "Geometry.h"

class Camera {
public:
	Camera(int w, int h, float aRatio, float vHeight, float cDist) :
		width(w), height(h), aspectRatio(aRatio), viewportHeight(vHeight), viewportWidth(setVWidth()), camDist(cDist)  {}


	const int width{};
	const int height{};
	const float aspectRatio{};
	const float viewportHeight{};
	const float viewportWidth{};
	const float camDist{};


	Vec3f origin{ 0.0f, 0.0f, 0.0f };
	Vec3f horizontal{ viewportWidth, 0.0f, 0.0f };
	Vec3f vertical{ 0.0f, viewportHeight, 0.0f };
	Vec3f lowerLeft = origin - horizontal / 2.0f - vertical / 2.0f - Vec3f(0, 0, camDist);


	float setVWidth() {
		return viewportHeight * aspectRatio;
	}

	void camLog() {
		std::cout << "Width: " << width << " " << "Height: " << height << "Aspect Ratio: " << aspectRatio << " Viewport Height and Width: " << viewportHeight << " " << viewportWidth << " " << "Camera Distance from plane: " << camDist << std::endl;
	}

	void planeLog() {
		origin.Log();
		horizontal.Log();
		vertical.Log();
		lowerLeft.Log();
	}

	

};
