#pragma once
#include <memory>
#include "Geometry.h"
#include "Material.h"

// 4. �����൥Ԫ����


class RenderableObject
{
protected:
	std::shared_ptr<Material const> _mat;
	std::shared_ptr<Geometry const> _geo;
public:
	RenderableObject();
	RenderableObject(std::shared_ptr<Material const> m, std::shared_ptr<Geometry const> g);
	Ray GetRefl(const Ray& r, const Vec3<double>& pos) const;
	Ray GetRefr(const Ray& r, const Vec3<double>& pos) const;
	void SetGeo(std::shared_ptr<Geometry const> );
	void SetMat(std::shared_ptr<Material const>);
	Color GetAlbedo() const;
	Color GetEmission() const;
	MatType GetMat() const;
	bool Intersect(const Ray& r, double& t) const;
};

