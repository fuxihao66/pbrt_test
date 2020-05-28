#include <memory>

#include "Camera.h"
#include "Material.h"
#include "SceneManager.h"
#include "Sphere.h"
#include "Glass.h"
#include "ObjWarehouse.h"





int main1() {
	// test intersection
	ObjWarehouse _objList;
	Ray r(Vec3<double>(-5, 4.99962, -3.51913), Vec3<double>(0.616031, 0.773086, -0.151143));

	auto matt1Mat = std::make_shared<Material>(MatType::Matt, Vec3<double>(0.2f, 0.4f, 0.2f));
	auto matt2Mat = std::make_shared<Material>(MatType::Matt, Vec3<double>(0.4f, 0.2f, 0.2f));
	auto matt3Mat = std::make_shared<Material>(MatType::Matt, Vec3<double>(0.2f, 0.2f, 0.4f));
	auto mattMat = std::make_shared<Material>(MatType::Matt, Vec3<double>(0.1f, 0.1f, 0.1f));

	auto planeLeft = std::make_shared<RenderableObject>(matt1Mat, std::make_shared<Plane>(Vec3<double>(-5, -5, -5), Vec3<double>(-5, 5, -5), Vec3<double>(-5, -5, 5)));
	auto planeRight = std::make_shared<RenderableObject>(matt1Mat, std::make_shared<Plane>(Vec3<double>(5, -5, -5), Vec3<double>(5, 5, -5), Vec3<double>(5, -5, 5)));
	auto planeFar = std::make_shared<RenderableObject>(matt2Mat, std::make_shared<Plane>(Vec3<double>(-5, -5, -5), Vec3<double>(5, -5, -5), Vec3<double>(5, 5, -5)));

	auto planeNear = std::make_shared<RenderableObject>(mattMat, std::make_shared<Plane>(Vec3<double>(-5, -5, 5), Vec3<double>(5, -5, 5), Vec3<double>(5, 5, 5)));

	auto planeTop = std::make_shared<RenderableObject>(matt3Mat, std::make_shared<Plane>(Vec3<double>(-5, 5, -5), Vec3<double>(5, 5, -5), Vec3<double>(5, 5, 5)));
	auto planeBottom = std::make_shared<RenderableObject>(matt3Mat, std::make_shared<Plane>(Vec3<double>(5, -5, 5), Vec3<double>(5, -5, -5), Vec3<double>(-5, -5, -5)));

	_objList.AddObject(planeLeft);
	_objList.AddObject(planeRight);
	_objList.AddObject(planeFar);
	_objList.AddObject(planeNear);
	_objList.AddObject(planeTop);
	_objList.AddObject(planeBottom);

	_objList.Intersect(r, 1);

	return 0;
}
int main() {
	resetRandomGen();

	auto mainCam = std::make_shared<Camera>(Vec3<double>(0, 0, 10));
	auto sm = std::make_shared<SceneManager>();
	
	
	sm->BuildDefaultScene();  
	auto glassMat = std::make_shared<Material>(MatType::Glass, Color(0.9f, 0.9f, 0.9f));
	auto glassBall = std::make_shared<RenderableObject>();
	glassBall->SetMat(glassMat);
	glassBall->SetGeo(std::make_shared<Sphere>(0.0f, -3.0f, 0.0f, 2.0f));
	sm->AddObject(glassBall);


	Config f = Config(300, 300, 500, 1);
	sm->SetConfig(f);
	sm->SetCamera(mainCam);
	auto image = sm->DoRenderPathTracing();

	if (image != nullptr) {
		image->Save("test.ppm");
	}

	return 0;
}