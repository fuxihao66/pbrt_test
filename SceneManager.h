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
#include "PerfectDiffuse.h"
#include "Mirror.h"
#include "Light.h"
#include "Plane.h"
#include "Sphere.h"
#include "LightLineSample.h"

class SceneManager
{
private:
	ObjWarehouse _objList;
	std::vector<std::shared_ptr<Light>> _lightList; // ֱ�ӹ���
	Config _f;
	std::shared_ptr<Camera const> _pCam;

	// path tracing 
	Color DoRenderPathTracing(const Ray&, int depth) const;

	// ����shadow ray ����ֱ�ӹ���
	Color DoRenderPathTracingWithDirectIllumination(const Ray&, int depth) const;

	//std::shared_ptr<Image> DoRenderPhotonMapping() const;
	Color DirectIlluminationPoint(const Ray& r, const Point& inscPoint, const Vec3<double>& inscNormal, std::shared_ptr<Material const> mat, size_t sampleNum) const;
	//Color DirectIlluminationLine(const Ray& r, const Point& inscPoint, const Vec3<double>& inscNormal, std::shared_ptr<Material const> mat) const;
public:
	SceneManager();

	bool BuildDefaultScene();  // 

	void AddObject(std::shared_ptr<const RenderableObject>);
	void SetConfig(const Config& f);
	void SetCamera(std::shared_ptr<const Camera>);
	std::shared_ptr<Image> DoRender() const;

};

