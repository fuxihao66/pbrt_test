#pragma once
#include <iostream>
#include "Ray.h"

class Geometry
{

public:
	virtual const Vec3<double> GetNormal(const Vec3<double>& pos)const = 0;
	virtual bool Intersect(const Ray&, double&)const = 0; // ����
	virtual bool Contain(const Vec3<double>& point) const = 0;
};

