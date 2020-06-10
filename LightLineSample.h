#pragma once
#include "utility.h"
#include "ObjWarehouse.h"
/* 相互引用 可能有问题*/

class LightLineSample
{
public:
	Point _L0;
	Vec3<double> _Ldir;
	Vec3<double> _Lvertical;
	double _length;
	std::vector<Vec2<double>> lineSeg;
	Color illumination;

	LightLineSample(const Point& l0, const Vec3<double>& ldir, const Vec3<double>& lvert, double leng, Color ill) {
		_L0 = l0;
		_Ldir = ldir;
		_Lvertical = lvert;
		_length = leng;
		illumination = ill;
	}

	// generate line segments
	/*void VisibilityTest(const ObjWarehouse& objList, const Point& inscPoint) {
	}*/

	void LineSegments() {

	}
};

