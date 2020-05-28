#pragma once
#include <vector>
#include <ctime>
#include <iostream>

class ObjWarehouse
{
private:
	std::vector<std::shared_ptr<RenderableObject const >> objList;
public:
	Color Intersect(const Ray& r, int depth)const {
		

		if (!r.Valid())
			return Color(0.0, 0.0, 0.0);

		double closestPoint = 0xFFFFF;
		

		std::shared_ptr<const RenderableObject> pClosestObject;
		for (auto it = objList.begin(); it != objList.end(); it++) {
			
			double t = 0xFFFFF;
			bool intersected = (*it)->Intersect(r, t);
			
			if (intersected && closestPoint > t) {
				closestPoint = t;
				pClosestObject = *it;
				//std::cout << "find intersection" << std::endl;
			}
		}
		int k;
		if (closestPoint == 0xFFFFF){
			/*std::cout << "depth is " << depth << std::endl;
			std::cout << r.ori.x << " " << r.ori.y << " " << r.ori.z << std::endl;
			std::cout << r.dir.x << " " << r.dir.y << " " << r.dir.z << std::endl;
			std::cout << "no intersection" << std::endl;
			std::cin >> k;*/
			return Color(0.0,0.0,0.0);
		}

		//std::cout << "t is " << closestPoint << std::endl;

		Color emission = pClosestObject->GetEmission();
		


		
		//// medium
		//double sigma_t = 0.3;
		//double sigma_s = 0.2;
		//double Te = exp(-sigma_t * closestPoint);


		double p = 0.6;
		double rnd = SampleRandomNum(0, 1);
		if (rnd > p)
			return emission;   // 终止trace

		

		// 采样光线
		/*double samplePoint = 0xFFFFF;
		while (samplePoint >= closestPoint) {
			double xi = SampleRandomNum(0.0, 1.0);
			samplePoint = -log(1 - xi) / sigma_t;
		}*/
		// 采样phase function
		//Ray scatterRay(r.ori+samplePoint*r.dir, SampleScatterDir());
		
		/*double scatterP = 0.4;
		rnd = SampleRandomNum(0, 1);
		Color Ls = Color(0.0, 0.0, 0.0);
		if (rnd < scatterP)
			Ls = (1/scatterP)*sigma_s/sigma_t* Intersect(scatterRay,depth+1);*/
		


		Vec3<double> albedo = pClosestObject->GetAlbedo();



		// estimator for sampling ray: g
		// with probability p to trace, 1-p to cut off, g'=1/pg(x) p; 0 1-p
		// E[g'] = 1/p g(x) * p + 0 = g(x)
		albedo = (1/p)*albedo;  

		MatType insecPointMat = pClosestObject->GetMat();

		
		Ray reflRay = pClosestObject->GetRefl(r, r.ori+ closestPoint * r.dir);
		Ray refrRay = pClosestObject->GetRefr(r, r.ori + closestPoint * r.dir);

		switch (insecPointMat) {
		case MatType::Emission:
			
			return emission;
			//return Te*emission + Ls;
			break;
		case MatType::Matt:
			
			//return Te*(emission + albedo * Intersect(reflRay, depth+1))+Ls;
			return (emission + albedo * Intersect(reflRay, depth + 1));
			break;
		case MatType::Specular:
			//return Te*(emission + albedo * Intersect(reflRay, depth + 1))+Ls;
			return (emission + albedo * Intersect(reflRay, depth + 1));
			break;
		case MatType::Glass:

			// 解frenel方程
			double reflRatio = 0.5;
			double refrRatio = 0.5;

			//return Te * (emission + reflRatio * albedo * Intersect(reflRay, depth + 1) + refrRatio * albedo * Intersect(refrRay, depth + 1)) + Ls;
			return (emission + reflRatio * albedo * Intersect(reflRay, depth + 1) + refrRatio * albedo * Intersect(refrRay, depth + 1));
		}

		
	}
	void AddObject(std::shared_ptr<RenderableObject const> obj) {
		objList.push_back(obj);
	}
};

