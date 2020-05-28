#include "RenderableObject.h"


RenderableObject::RenderableObject() {

}
RenderableObject::RenderableObject(std::shared_ptr<Material const> m, std::shared_ptr<Geometry const> g) {
	_mat = m;
	_geo = g;
}
bool RenderableObject::Intersect(const Ray& r, double& t) const{
	
	return _geo->Intersect(r, t);
}

MatType RenderableObject::GetMat() const{
	return _mat->GetMat();
}

Color RenderableObject::GetAlbedo() const{
	return _mat->GetAlbedo();
}

Color RenderableObject::GetEmission() const {
	return _mat->GetEmission();
}

void RenderableObject::SetGeo(std::shared_ptr<Geometry const> g) {
	_geo = g;
}
void RenderableObject::SetMat(std::shared_ptr<Material const> m) {
	_mat = m;
}

Ray RenderableObject::GetRefl(const Ray& r, const Vec3<double>& pos) const{
	// Ray non valid
	Vec3<double> norm = _geo->GetNormal(pos);


	return _mat->ReflImpSampling(r, pos, norm);
}
Ray RenderableObject::GetRefr(const Ray& r, const Vec3<double>& pos) const{
	// Ray non valid
	Vec3<double> norm = _geo->GetNormal(pos);
	bool inside = false;
	if (_geo->Contain(r.ori)) {
		inside = true;
	}


	return _mat->RefrImpSampling(r, pos, norm, inside);
	
}