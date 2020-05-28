#pragma once

#include <cmath>
#include <iostream>
#include "utility.h"
#include "Image.h"
#include "Ray.h"
#include "RenderableObject.h"
#include "ObjWarehouse.h"
#include "Camera.h"
#include "Material.h"
#include "Light.h"
#include "Plane.h"
#include "Sphere.h"

class SceneManager
{
private:
	// ObjWarehouse for path tracing
	ObjWarehouse _objList;
	Config _f;
	std::shared_ptr<Camera const> _pCam;
public:
	SceneManager();

	bool BuildDefaultScene();  // 

	void AddObject(std::shared_ptr<const RenderableObject>);
	void SetConfig(const Config& f);
	void SetCamera(std::shared_ptr<const Camera>);
	std::shared_ptr<Image> DoRenderPathTracing() const;
	//std::shared_ptr<Image> DoRenderPhotonMapping() const;
};

