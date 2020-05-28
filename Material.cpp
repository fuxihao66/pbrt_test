#include "Material.h"


Material::Material() {
	matType = MatType::Matt;
}
Material::Material(MatType m, const Vec3<double>& _albedo) {
	matType = m;
	if (m == MatType::Emission)
		emit = _albedo;
	else
		albedo = _albedo;
}
Color Material::GetEmission() const{
	return emit;
}
Color Material::GetAlbedo()const{
	return albedo;
}

MatType Material::GetMat()const {
	return matType;
}

void Material::SetAlbedo(const Color& v) {
	albedo = v;
}
void Material::SetEmission(const Color& c) {
	emit = c;
}
void Material::SetRefrRatio(double ratio) {
	refrRatio = ratio;
}

Ray Material::ReflImpSampling(const Ray& r, const Vec3<double>& pos, const Vec3<double>& normal) const{

	Vec3<double> modified_norm;

	if (normal.dot(r.dir) > 0)  //
		modified_norm = -1 * normal;
	else
		modified_norm = normal;

	Ray reflRay;
	reflRay.ori = pos;
	switch (matType) {
	case MatType::Specular:
		reflRay.dir = 2 * (r.dir - r.dir.dot(modified_norm) * modified_norm) - r.dir;
		reflRay.dir.normalize();
		break;
	case MatType::Matt:

		reflRay.dir = SampleOnSemiSphere(modified_norm);

		break;
	case MatType::Glass:
		reflRay.dir = 2 * (r.dir - r.dir.dot(modified_norm) * modified_norm) - r.dir;
		reflRay.dir.normalize();

		break;
	case MatType::Emission:
		reflRay.SetValidValue(false);
		break;
	}

	return reflRay;
}

Ray Material::RefrImpSampling(const Ray& r, const Vec3<double>& pos, const Vec3<double>& normal, bool inside = false) const{

	Vec3<double> modified_norm;

	if (normal.dot(r.dir) > 0) {  //
		modified_norm = -1 * normal;

	}
	else
		modified_norm = normal;


	double realRefrRatio = refrRatio;
	if (inside) {
		realRefrRatio = 1 / realRefrRatio;
	}


	Ray refrRay;
	refrRay.ori = pos;
	switch (matType) {
	case MatType::Glass:
	{
		double cosTheta = -1 * modified_norm.dot(r.dir);
		double refrSin = cos2sin(cosTheta) / realRefrRatio;
		if (refrSin >= 1)
			refrRay.SetValidValue(false);
		else {
			double refrCos = sin2cos(refrSin);
			Vec3<double> horizontal = refrSin * (r.dir + cosTheta * modified_norm).normalize();
			refrRay.dir = horizontal - refrCos * modified_norm;
		}

		break;
	}
	default:
		refrRay.SetValidValue(false);
	}

	return refrRay;
}

double Material::GetBTDF() {
	return 0.0f;
}
double Material::GetBSDF() {
	return 0.0f;
}