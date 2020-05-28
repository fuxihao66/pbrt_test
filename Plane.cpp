#include "Plane.h"

Plane::Plane() {

}

Plane::Plane(const Vec3<double>& _p1, const Vec3<double>& _p2, const Vec3<double>& _p3) {
	p1 = _p1;
	p2 = _p2;
	p3 = _p3;
	p2p1 = p1 - p2;
	p2p3 = p3 - p2;
	w = p2p1.length();
	h = p2p3.length();
	normal = p2p3.OuterProd(p2p1); //
	normal.normalize();
}

const Vec3<double> Plane::GetNormal(const Vec3<double>& pos) const{
	return normal;

}
bool Plane::Intersect(const Ray& r, double& t)const {
	if (normal.Parallel(r.dir))
		return false;

	t = (normal.dot(p2) - normal.dot(r.ori)) / normal.dot(r.dir);
	
	if (t < 0)
		return false;

	if (abs(t) < EPSILON)
		return false;

	Vec3<double> p2InsecPoint = r.ori + t * r.dir - p2;


	double innerProduct1 = p2InsecPoint.dot(p2p1);
	double innerProduct2 = p2InsecPoint.dot(p2p3);


	if (innerProduct1 > w* w || innerProduct1 < 0 || innerProduct2 > h * h || innerProduct2 < 0)
		return false;
	else
		return true;

}

bool Plane::Contain(const Vec3<double>& point) const {
	return false;
}