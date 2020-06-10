#include "Phong.h"


Phong::Phong(MatType m, const Vec3<double>& _albedo, UINT n) 
	: Material(m, _albedo)
{
	_phongCoeff = n;
}

Ray Phong::ReflImpSampling(const Ray& r, const Vec3<double>& pos, const Vec3<double>& normal, double& pdf, double& brdf, Color& albedo) const {

	albedo = GetAlbedo();

	return ReflImpSampling(r, pos, normal, pdf, brdf);


}

Ray Phong::RefrImpSampling(const Ray& r, const Vec3<double>& pos, const Vec3<double>& normal, double& pdf, double& brdf, Color& albedo) const {
	return Ray();
}


Ray Phong::ReflImpSampling(const Ray& r, const Vec3<double>& pos, const Vec3<double>& normal, double& pdf, double& brdf) const {

	Vec3<double> modifiedNorm = normal;
	if (normal.dot(r.dir) > 0)
		modifiedNorm = -1 * normal;


	Vec3<double> mirrorReflVec = sqrt(2) * r.dir + 2.0 * modifiedNorm;
	mirrorReflVec.normalize();



	Vec3<double> newDir = SamplePhongRefl(modifiedNorm, mirrorReflVec, _phongCoeff);
	Ray newRay(pos, newDir);

	// return by reference
	brdf = BRDF(r.dir, newRay.dir, modifiedNorm);
	pdf = brdf * (_phongCoeff + 1) / (_phongCoeff + 2);

	return newRay;


}

Ray Phong::RefrImpSampling(const Ray& r, const Vec3<double>& pos, const Vec3<double>& normal, double& pdf, double& brdf) const {
	return Ray();
}

// no transmission
double Phong::BTDF(const Vec3<double>& wi, const Vec3<double>& wo, const Vec3<double>& norm) const {
	return 0.0;
}

double Phong::BRDF(const Vec3<double>& wi, const Vec3<double>& wo, const Vec3<double>& norm) const {
	
	Vec3<double> modifiedNorm = norm;
	if (norm.dot(wi) > 0)
		modifiedNorm = -1 * norm;

	Vec3<double> mirrorReflVec = sqrt(2) * wi + 2.0 * modifiedNorm;
	mirrorReflVec.normalize();

	return  (_phongCoeff +2)/(2*PI)* pow(wo.dot(mirrorReflVec), _phongCoeff);
}

//Color Phong::AnalyticLineIllumination(const std::vector<Vec2<double>>& lineSeg, const LightLineSample& lls, const Point& inscPoint, const Vec3<double>& inscNorm, const Vec3<double>& lightNorm, const Vec3<double>& incidentDir)const {
//		
//
//	// norm ·½Ïò
//
//}

