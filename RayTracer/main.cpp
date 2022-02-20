#include <iostream>
#include <vector>
#include <assert.h>
#include <memory>
#include "Geometry.h"
#include "Sphere.h"
#include "Camera.h"
#include "Lights.h"



float Dot(const Vec3f& v1, const Vec3f& v2)  {
	return v1.x() * v2.x() + v1.y() * v2.y() + v1.z() * v2.z();
}

float rayIntersect(const Sphere& s1, const Vec3f& origin, const Vec3f& dir) {
	Vec3f ld1 = origin - s1.s_Center;	
	float a = Dot(dir, dir);
	float b = 2.0f * Dot(ld1, dir);
	float c = Dot(ld1, ld1) - (s1.s_Radius * s1.s_Radius);
	float discriminant = b * b - 4 * a * c;

	float sq_discriminant = sqrtf(discriminant);
	// Closest point on ray
	float distance1 = (-b - sq_discriminant) / (2.0f * a);
	if (distance1 > 0.1f) {return distance1;}

	//distance1 = (-b + sq_discriminant) / (2.0f * a);
	//if (distance1 > 0.1f) { return distance1;}

	return -1.0f;

}


Vec3f reflect_Ray(const Vec3f& normalP, const Vec3f& lightDir) {
	return lightDir - (normalP * 2.0f * Dot(lightDir, normalP));

}


Vec3f cast_Ray(const std::vector<std::shared_ptr<Sphere>>&s_List, const std::vector<std::shared_ptr<Lights>>&l_List, const Vec3f& origin, const Vec3f& dir, unsigned int f_Depth) 
{

	if (f_Depth > 8) { return { 0.0f, 0.0f, 0.4f }; }

	float dist = std::numeric_limits<float>::max();
	int hIndex = -1;
	Vec3f hp, N, c_Dir, c_Origin, r_Dir, r_Origin;
	c_Dir = dir;
	c_Origin = origin;
	
	for (int i = 0; i < s_List.size(); i++) {
		float m = rayIntersect(*s_List[i], c_Origin, c_Dir);
		if (m > 1.0f && m < dist) {
			dist = m;
			hp = c_Origin + c_Dir * m;
			N = hp - s_List[i]->s_Center;
			N.unitVec();
			hIndex = i;
		}
	}

	if (hIndex == -1){return { 0.0f, 0.0f, 0.4f };}
	r_Dir = reflect_Ray(c_Dir, N).unitVec();
	r_Origin = (Dot(r_Dir, N) < 0 ? hp + N * 1e-3: hp - N * 1e-3);
	Vec3f depthColor = cast_Ray(s_List, l_List, r_Origin, r_Dir, f_Depth + 1);

	float dif_LI = 0, spec_LI = 0;

	for (int i = 0; i < l_List.size(); i++) {
		Vec3f lp = l_List[i]->pos - hp;
		lp.unitVec();
		float lp_length = (l_List[i]->pos - hp).length();

		Vec3f shadow_Origin = hp, shadow_hp, shadow_N;
		int sIndex = -1;
		dist = std::numeric_limits<float>::max();
		for (int j = 0; j < s_List.size(); j++) {
			float m = rayIntersect(*s_List[j], shadow_Origin, lp);
			if (m > 0.0f && m < dist) {
				dist = m;
				shadow_hp = shadow_Origin + lp * m;
				shadow_N = shadow_hp - s_List[j]->s_Center;
				shadow_N.unitVec();
				sIndex = j;
			}
		}

		if (sIndex != -1 && (shadow_hp - shadow_Origin).length() < lp_length) { continue; }

		dif_LI += l_List[i]->intensity * std::max(0.0f, Dot(N, lp));
		Vec3f rRay = reflect_Ray(N, lp);
		rRay.unitVec();
		spec_LI += powf(std::max(0.0f, Dot(rRay, dir)), s_List[hIndex]->specularExponent) * l_List[i]->intensity;
	}
	return ((s_List[hIndex]->s_Material * dif_LI * s_List[hIndex]->albedo[0]) + (Vec3f(1.0f, 1.0f, 1.0f) * spec_LI * s_List[hIndex]->albedo[1]) + (depthColor * s_List[hIndex]->albedo[2]));

	// Iterate over all spheres
	//for (int i = 0; i < s_List.size(); i++) {
	//	//if (rayIntersect(*s_List[i], origin, dir)) { return s_List[i]->s_Material; }
	//	float m = rayIntersect(*s_List[i], origin, dir);
	//	float dif_LI = 0, spec_LI = 0;
	//	if (m > 0.0f){
	//		for (int j = 0; j < l_List.size(); j++) {
	//			Vec3f hp = origin + dir * m;
	//			Vec3f lp = l_List[j]->pos - hp;
	//			lp.unitVec();
	//			Vec3f N = hp - s_List[i]->s_Center;
	//			N.unitVec();
	//			dif_LI += l1.intensity * std::max(0.0f, Dot(N, lp));
	//			Vec3f rRay = reflect_Ray(N, lp);
	//			rRay.unitVec();
	//			spec_LI += powf(std::max(0.0f, Dot(rRay, dir)), s_List[i]->specularExponent) * l1.intensity;
	//		}
	//		return ((s_List[i]->s_Material * dif_LI) + (Vec3f(1.0f, 1.0f, 1.0f) * spec_LI));
	//	}				
	//}
	//return { 0.0f, 0.0f, 0.4f };
		
}

void Render() {
	
	/*Camera c1(800, 450, 16.0f / 9.0f, 2.0f, 1.0f);*/
	Camera c1(800, 600, 4.0f / 3.0f, 2.0f, 1.0f);

	std::cout << "P3" << std::endl;
	std::cout << c1.width << " " << c1.height << std::endl;
	std::cout << 255 << std::endl;

	//Sphere s1{ {-3.5, 0.0f, -5.5}, 1.5, {0.4, 0.4, 0.3}, {0.3, 0.3, 0.0}, 50.0f }; 
	std::shared_ptr<Sphere> s1 = std::make_shared<Sphere>(Vec3f{ -1.5, -1.0f, -5.5 }, 1.5, Vec3f{ 0.4f, 1.0f, 1.0f }, Vec3f{ 0.0f, 10.0f, 0.8f }, 1425.0f);
	std::shared_ptr<Sphere> s2 = std::make_shared<Sphere>(Vec3f{ 4.5, -0.5, -10.5 }, 2.0, Vec3f(0.6f, 0.3f, 0.1f), Vec3f{ 0.9f, 0.1f, 0.4f}, 25.0);
	std::shared_ptr<Sphere> s3 = std::make_shared<Sphere>(Vec3f{ 4.5, -5.5, -10.5 }, 2.0, Vec3f(1.0f, 0.8f, 1.0f), Vec3f{ 0.1f, 5.1f, 0.1f }, 18.0f);
	std::shared_ptr<Sphere> s4 = std::make_shared<Sphere>(Vec3f{ 4.5, 5.5, -10.5 }, 2.0, Vec3f(0.6f, 0.3f, 0.1f), Vec3f{ 0.2f, 0.2f, 0.3f }, 50.0f);
	std::shared_ptr<Sphere> s5 = std::make_shared<Sphere>(Vec3f{ -3.5, 1.5f, -4.0f }, 1.0f, Vec3f(0.9f, 0.1f, 0.0f), Vec3f{ 0.3f, 0.3f, 0.1f }, 20.0f);
	//std::shared_ptr<Sphere> s6 = std::make_shared<Sphere>(Vec3f{ -5.5, 0.0f, -12.5 }, 1.0, Vec3f{ 0.8, 0.2, 0.0 },Vec3f{ 0.3, 0.3, 0.0 }, 20.0f);

	//std::shared_ptr<Sphere> s1 = std::make_shared<Sphere>(Vec3f{ 0.0f, 0.0f, -5.0f }, 2.0f, Vec3f{ 1.0f, 1.0f, 1.0f }, Vec3f{ 0.0f, 10.0f, 0.8f }, 1425.0f);
	//std::shared_ptr<Sphere> s2 = std::make_shared<Sphere>(Vec3f{ 0.5f, -1.0f, -1.5f }, 0.5f, Vec3f(0.6f, 0.3f, 0.1f), Vec3f{ 0.9f, 0.1f, 0.4f }, 25.0);
	//std::shared_ptr<Sphere> s3 = std::make_shared<Sphere>(Vec3f{ -4.5f, 0.0f, -5.5f }, 1.5f, Vec3f(0.6f, 0.3f, 0.1f), Vec3f{ 0.9f, .1f, 0.4f }, 128.0);
	//std::shared_ptr<Sphere> s3 = std::make_shared<Sphere>(Vec3f{ 4.5, -5.5, -10.5 }, 2.0, Vec3f(0.6f, 0.3f, 0.1f), Vec3f{ 0.1f, 5.1f, 0.1f }, 18.0f);
	//std::shared_ptr<Sphere> s4 = std::make_shared<Sphere>(Vec3f{ 4.5, 5.5, -10.5 }, 2.0, Vec3f(0.6f, 0.3f, 0.1f), Vec3f{ 0.2f, 0.2f, 0.3f }, 50.0f);

	std::shared_ptr<Lights> l1 = std::make_unique<Lights>(Vec3f{ -10.0f, 10.0f, 10.0f }, 2.6f);
	std::shared_ptr<Lights> l2 = std::make_unique<Lights>(Vec3f{ 50.0f, 30.0f, -25.0f }, 1.1f);
	std::shared_ptr<Lights> l3 = std::make_unique<Lights>(Vec3f{30.0f, 20.0f, 30.0f}, 1.8f);

	
	std::vector<std::shared_ptr<Sphere>>sphereList;
	sphereList.push_back(s1);
	sphereList.push_back(s2);
	sphereList.push_back(s3);
	sphereList.push_back(s4);
	sphereList.push_back(s5);
	//sphereList.push_back(&s6);

	std::vector<std::shared_ptr<Lights>>lightList;
	lightList.push_back(l1);
	lightList.push_back(l2);
	//lightList.push_back(&l3);

	

	for (int j = c1.height - 1; j >= 0; --j) {
		for (int i = 0; i < c1.width; ++i) {
			//Vec3f v1(j / (float)height, i / (float)width, 0.25);
			// So we construct a ray to send through the viewport 			
			float u = (float)i / (c1.width - 1);
			float v = (float)j / (c1.height - 1);
			Vec3f dir = c1.lowerLeft + (c1.horizontal * u) + (c1.vertical * v) - c1.origin;		
			dir.unitVec();
			unsigned int f_Depth = 0;
			Vec3f f_Color = cast_Ray(sphereList, lightList, c1.origin, dir, f_Depth);
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