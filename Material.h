#pragma once
#include <iostream>
#include "utility.h"
#include "Ray.h"

class Material
{
protected:
	// default albedo for glass
	Color albedo;
	double refrRatio = 1.4;
	Color emit;
	MatType matType;
public:
	Material();
	Material(MatType m, const Vec3<double>& _albedo);

	Color GetEmission() const;
	Color GetAlbedo() const;
	void SetAlbedo(const Color& v);
	void SetEmission(const Color& c);
	void SetRefrRatio(double ratio);
	MatType GetMat() const;
	// ��Ҫ�Բ���������������߶�brdf����Ҫ�Բ���
	// 
	// ����ķ���Ϊ����ķ���  ������Ҫ������Ҫ���з�ת
	Ray ReflImpSampling(const Ray& r, const Vec3<double>& pos, const Vec3<double>& normal) const;

	// �����������䲻ͬ
	Ray RefrImpSampling(const Ray& r, const Vec3<double>& pos, const Vec3<double>& normal, bool inside) const;

	double GetBTDF();
	double GetBSDF();
};

