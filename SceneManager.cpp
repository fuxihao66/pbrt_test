#include "SceneManager.h"


SceneManager::SceneManager() {

}
void SceneManager::SetCamera(std::shared_ptr<Camera const > cam) {
	_pCam = cam;
}

bool SceneManager::BuildDefaultScene() {

	auto matt1Mat = std::make_shared<Material>(MatType::Matt, Vec3<double>(0.75f,0.75f,0.75f));
	auto matt2Mat = std::make_shared<Material>(MatType::Matt, Vec3<double>(0.75f, 0.25f, 0.25f));
	auto matt3Mat = std::make_shared<Material>(MatType::Matt, Vec3<double>(0.25f, 0.25f, 0.75f));
	auto mattMat = std::make_shared<Material>(MatType::Matt, Vec3<double>(0.1f, 0.1f, 0.1f));

	auto planeLeft = std::make_shared<RenderableObject>(matt2Mat, std::make_shared<Plane>(Vec3<double>(-5,-5,-5), Vec3<double>(-5,5,-5), Vec3<double>(-5, -5, 5)));
	auto planeRight = std::make_shared<RenderableObject>(matt3Mat, std::make_shared<Plane>(Vec3<double>(5, -5, -5), Vec3<double>(5, 5, -5), Vec3<double>(5, -5, 5)));
	auto planeFar = std::make_shared<RenderableObject>(matt1Mat, std::make_shared<Plane>(Vec3<double>(-5,-5,-5), Vec3<double>(5,-5,-5), Vec3<double>(5,5,-5)));

	auto planeNear = std::make_shared<RenderableObject>(mattMat, std::make_shared<Plane>(Vec3<double>(-5, -5, 5), Vec3<double>(5, -5, 5), Vec3<double>(5, 5, 5)));

	auto planeTop = std::make_shared<RenderableObject>(matt1Mat, std::make_shared<Plane>(Vec3<double>(-5,5,-5), Vec3<double>(5,5,-5), Vec3<double>(5,5,5)));
	auto planeBottom = std::make_shared<RenderableObject>(matt1Mat, std::make_shared<Plane>(Vec3<double>(5,-5,5), Vec3<double>(5,-5,-5), Vec3<double>(-5,-5,-5)));
	auto emissionMat = std::make_shared<Material>(MatType::Emission, Vec3<double>(18.0f, 18.0f, 18.0f));
	auto emissionMat2 = std::make_shared<Material>(MatType::Emission, Vec3<double>(9.0f, 10.0f, 10.0f));
	
	auto mainLight = std::make_shared<Light>(emissionMat, std::make_shared<Plane>(Vec3<double>(-2, 4.9, 2), Vec3<double>(-2, 4.9, -2),Vec3<double>(2, 4.9, -2))  );
	auto mainLight2 = std::make_shared<Light>(emissionMat2, std::make_shared<Plane>(Vec3<double>(4.9999, 5, -5),Vec3<double>(4.9999, -5, -5), Vec3<double>(4.9999, -5, 5)));
	auto mainLight3 = std::make_shared<Light>(emissionMat, std::make_shared<Plane>(Vec3<double>(-4.9999, 5, -5), Vec3<double>(-4.9999, -5, -5), Vec3<double>(-4.9999, -5, 5)));
	auto backLight = std::make_shared<Light>(emissionMat, std::make_shared<Plane>(Vec3<double>(-5, -5, -4.99), Vec3<double>(5, -5, -4.99), Vec3<double>(5, 5, -4.99)));
	_objList.AddObject(planeLeft);
	_objList.AddObject(planeRight);
	_objList.AddObject(planeFar);
	_objList.AddObject(planeNear);
	_objList.AddObject(planeTop);
	_objList.AddObject(planeBottom);
	_objList.AddObject(mainLight);
	//_objList.AddObject(mainLight2);
	//_objList.AddObject(mainLight3);
	//_objList.AddObject(backLight);

	return true;
}

void SceneManager::AddObject(std::shared_ptr<RenderableObject const> obj) {
	_objList.AddObject(obj);
}
void SceneManager::SetConfig(const Config& f) {
	_f = f;
}

std::shared_ptr<Image> SceneManager::DoRenderPathTracing() const {
	auto img = std::make_shared<Image>(_f.WIDTH, _f.HEIGHT);

	//auto eachThreadRow = ceil((double)(_f.WIDTH) / _f.THREAD);
	


	// plane is on z=5
	double widthPixel = 10 / (double)_f.WIDTH;
	double heightPixel = 10 / (double)_f.HEIGHT;
	for (UINT col = 0; col < _f.HEIGHT; col++) {
		for (UINT row = 0; row < _f.WIDTH; row++) {

			// subpixel 
			
			Color finalColor;
			for (int sy = 0; sy < 2; sy++)     // 2x2 subpixel rows 
				for (int sx = 0; sx < 2; sx++) {        // 2x2 subpixel cols 
					Color color;
					for (UINT sampIndex = 0; sampIndex < _f.SSP; sampIndex++) {

						// samp position

						double randomWidthOffset = SampleRandomNum(0, widthPixel / 2) + widthPixel / 2 * sx;
						double randomHeightOffset = SampleRandomNum(0, heightPixel / 2) + heightPixel / 2 * sy;
						/*Vec3<double> randomPoint(-5.0f + row * widthPixel + widthPixel / 2, -(-5.0f + col * heightPixel + heightPixel / 2), 5.0f);*/
						Vec3<double> randomPoint(-5.0f + row * widthPixel + randomWidthOffset, -(-5.0f + col * heightPixel + randomHeightOffset), 4.9f);

						auto dir = randomPoint - _pCam->position;
						dir.normalize();
						Ray r(randomPoint, dir);
						Color c = _objList.Intersect(r, 1);
						color = color + c;
					}
					color = (1.0 / _f.SSP) * color;
					
					// 这一个是核心
					finalColor = finalColor + 0.25 * Vec3<double>(clamp(color.x), clamp(color.y), clamp(color.z));
				}
			img->SetColor(row, col, finalColor);

			std::cout << "pixel" << row << " " << col << " finished" << std::endl;
		}
	}
	
	return img;
}

//std::shared_ptr<Image> SceneManager::DoRenderPhotonMapping() const {
//	auto img = std::make_shared<Image>(_f.WIDTH, _f.HEIGHT);
//
//	auto globalPhotonMap = std::make_shared<PhotonMap>();
//	auto causticPhotonMap = std::make_shared<PhotonMap>();
//	UINT photonNum = 10000;
//	double lightPower = 100;
//	// pass one
//	for (UINT i = 0; i < photonNum; i++) {
//
//	}
//
//
//	// pass two
//	for (UINT row = 0; row < _f.WIDTH; row++) {
//		for (UINT col = 0; col < _f.HEIGHT; col++) {
//			// subpixel 
//			Color color;
//			for (UINT sampIndex = 0; sampIndex < _f.SSP; sampIndex++) {
//				// samp position
//
//				Ray r(Vec3<double>(0.0, 0.0, 0.0), Vec3<double>(1.0, 1.0, 1.0));
//				Color c = _objList.PhotonPass(r, globalPhotonMap, causticPhotonMap);
//				color = color + c;
//			}
//			color = (1 / _f.SSP) * color;
//			img->SetColor(row, col, color);
//		}
//	}
//
//}