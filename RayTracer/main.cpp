#include <iostream>
#include <vector>
#include <assert.h>
#include "Geometry.h"
#include "Sphere.h"
#include "Camera.h"
#include "Lights.h"

float Dot(const Vec3f& v1, const Vec3f& v2)  {
	return v1.x() * v2.x() + v1.y() * v2.y() + v1.z() * v2.z();
}

float rayIntersect(const Sphere& s1, const Vec3f& origin, const Vec3f& dir) {
	Vec3f l1 = origin - s1.s_Center;	
	float a = Dot(dir, dir);
	float b = 2.0f * Dot(l1, dir);
	float c = Dot(l1, l1) - (s1.s_Radius * s1.s_Radius);
	float discriminant = b * b - 4 * a * c;

	float sq_discriminant = sqrtf(discriminant);
	// Closest point on ray
	float distance1 = (-b - sq_discriminant) / (2.0f * a);
	if (distance1 > 0.1f) {return distance1;}

	distance1 = (-b + sq_discriminant) / (2.0f * a);
	if (distance1 > 0.1f) { return distance1;}

	return -1.0f;

}


Vec3f reflect_Ray(const Vec3f& normalP, const Vec3f& lightDir) {
	return lightDir - normalP * 2.0f * Dot(lightDir, normalP);

}


Vec3f cast_Ray(std::vector<Sphere*>&s_List, std::vector<Lights*>l_List, const Vec3f& origin, const Vec3f& dir, const Lights& l1) {
	// Iterate over all spheres
	for (int i = 0; i < s_List.size(); i++) {
		//if (rayIntersect(*s_List[i], origin, dir)) { return s_List[i]->s_Material; }
		float m = rayIntersect(*s_List[i], origin, dir);
		float dif_LI = 0, spec_LI = 0;
		if (m > 0.0f){
			for (int j = 0; j < l_List.size(); j++) {
				Vec3f hp = origin + dir * m;
				Vec3f lp = l_List[j]->pos - hp;
				lp.unitVec();
				Vec3f N = hp - s_List[i]->s_Center;
				N.unitVec();
				dif_LI += l1.intensity * std::max(0.0f, Dot(N, lp));
				Vec3f rRay = reflect_Ray(N, lp);
				rRay.unitVec();
				spec_LI += powf(std::max(0.0f, Dot(rRay, dir)), s_List[i]->specularExponent) * l1.intensity;
			}
			return ((s_List[i]->s_Material * dif_LI) + (Vec3f(1.0f, 1.0f, 1.0f) * spec_LI));
		}				
	}
	return { 0.0f, 0.0f, 0.4f };
		
}

void Render() {
	
	/*Camera c1(800, 450, 16.0f / 9.0f, 2.0f, 1.0f);*/
	Camera c1(800, 600, 4.0f / 3.0f, 2.0f, 1.0f);

	std::cout << "P3" << std::endl;
	std::cout << c1.width << " " << c1.height << std::endl;
	std::cout << 255 << std::endl;

	Sphere s1{ {-3.5, 0.0f, -5.5}, 1.5, {0.4, 0.4, 0.3}, {0.3, 0.3, 0.0}, 50.0f };
	Sphere s2{ {4.5, -0.5, -10.5}, 2.0, {0.3, 0.1, 0.1}, {0.9, 0.1, 0.0}, 25.0f };
	Sphere s3{ {4.5, -5.5, -10.5}, 2.0, {0.3, 0.1, 0.1}, {0.1, 0.1, 0.0}, 18.0f };
	Sphere s4{ {4.5, 5.5, -10.5}, 2.0, {0.3, 0.1, 0.1}, {0.2, 0.2, 0.0}, 50.0f };
	Sphere s5{ {2.5, 1.5f, -3.5f}, 1.5, {0.1, 0.4, 0.1}, {0.3, 0.3, 0.0}, 20.0f };
	Sphere s6{ {-5.5, 0.0f, -12.5}, 1.0, {0.8, 0.2, 0.0}, {0.3, 0.3, 0.0}, 20.0f };



	Lights l1({ -10.0f, 10.0f, 10.0f }, 1.6f);
	Lights l2({ 50.0f, 30.0f, -25.0f }, 1.1f);
	Lights l3({ 30.0f, 20.0f, 30.0f }, 1.8f);

	
	std::vector<Sphere*>sphereList;
	sphereList.push_back(&s1);
	sphereList.push_back(&s2);
	sphereList.push_back(&s3);
	sphereList.push_back(&s4);
	sphereList.push_back(&s5);
	sphereList.push_back(&s6);

	std::vector<Lights*>lightList;
	lightList.push_back(&l1);
	lightList.push_back(&l2);
	//lightList.push_back(&l3);

	

	for (int j = c1.height - 1; j >= 0; --j) {
		for (int i = 0; i < c1.width; ++i) {
			//Vec3f v1(j / (float)height, i / (float)width, 0.25);
			// So we construct a ray to send through the viewport 			
			float u = (float)i / (c1.width - 1);
			float v = (float)j / (c1.height - 1);
			Vec3f dir = c1.lowerLeft + (c1.horizontal * u) + (c1.vertical * v) - c1.origin;		
			dir.unitVec();
			Vec3f f_Color = cast_Ray(sphereList, lightList, c1.origin, dir, l1);
			float maxVal = std::max(f_Color[0], std::max(f_Color[1], f_Color[2]));
			if (maxVal > 1) { maxVal = 1.0f / maxVal; f_Color *= maxVal; }
			//std::cout << (int)(255.9f * std::max(0.0f, std::min(1.0f, f_Color.x()))) << " " << (int)(255.9f * std::max(0.0f, std::min(1.0f, f_Color.y()))) << " " << (int)(255.9f * std::max(0.0f, std::min(1.0f, f_Color.z()))) << std::endl;
			std::cout << (int)(255.9f * f_Color.x()) << " " << (int)(255.9f * f_Color.y()) << " " << (int)(255.9f * f_Color.z()) << std::endl;


		}
	}

}




int main() {


	// Let's make this really cool
	// But first, fundamentally understand what we're doing here
	// Firstly, we're gonna demo writing to a ppm file, 256 x 256 format
	
	Render();

	return 0;
}