#pragma once
#include <cmath>
#include <ctime>

#include "happy.h"

typedef unsigned int UINT;
#define PI 3.1415926
#define EPSILON (10e-7)

struct Config {
	UINT WIDTH;
	UINT HEIGHT;
	UINT SSP;
	UINT THREAD;
	Config() {
		WIDTH = 480;
		HEIGHT = 480;
		SSP = 100;
		THREAD = 1;
	}
	Config(UINT w, UINT h, UINT s, UINT t) {
		WIDTH = w;
		HEIGHT = h;
		SSP = s;
		THREAD = t;
	}
};

template <class T>
struct Vec2 {
	T x;
	T y;
	Vec2() {
		x = static_cast<T>(0);
		y = static_cast<T>(0);;
	}
	Vec2(const T& _x, const T& _y){
		x = _x;
		y = _y;
	}
};


template <class T>
struct Vec3 {
	T x;
	T y;
	T z;
	Vec3() {
		x = static_cast<T>(0);
		y = static_cast<T>(0);
		z = static_cast<T>(0);
	}
	Vec3(const T& _x, const T& _y, const T& _z) {
		x = _x;
		y = _y;
		z = _z;
	}	

	bool Parallel(const Vec3<double>& v) const{
		auto mul = (*this).OuterProd(v);
		if (abs(mul.x) < EPSILON && abs(mul.y) < EPSILON && abs(mul.z) < EPSILON)
			return true;
		else
			return false;
	}

	 Vec3<T>& normalize() {
		double norm = static_cast<double>(sqrt(x * x + y * y + z * z));
		x = static_cast<double>(x / norm);
		y = static_cast<double>(y / norm);
		z = static_cast<double>(z / norm);

		return *this;
	}
	T dot(const Vec3<T>& anotherVec) const{
		return x * anotherVec.x + y * anotherVec.y + z * anotherVec.z;
	}

	 Vec3<T> OuterProd(const Vec3<T>& v2) const {
		return Vec3<T>(y * v2.z - v2.y * z, v2.x * z - x * v2.z, x * v2.y - v2.x * y);
	}

	T length() const{
		return sqrt(x * x + y * y + z * z);
	}

	friend  Vec3<T> operator *(T para, const Vec3<T>& v2) {
		return Vec3<T>(para * v2.x, para * v2.y, para * v2.z);
	}

	friend  Vec3<T> operator +(const Vec3<T>& v1, const Vec3<T>& v2) {
		return Vec3<T>(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
	}

	friend  Vec3<T> operator -(const Vec3<T>& v1, const Vec3<T>& v2) {
		return Vec3<T>(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
	}

	friend  Vec3<T> operator *(const Vec3<T>& v1, const Vec3<T>& v2) {
		return Vec3<T>(v1.x * v2.x, v1.y*v2.y, v1.z*v2.z);
	}
};

typedef Vec3<double> Color;
typedef Vec3<double> Point;
typedef UINT Mat;

inline double distance(const Vec3<double>& p1, const Vec3<double>& p2) {
	return (p1 - p2).length();
}

inline double clamp(double x) { return x < 0 ? 0 : x>1 ? 1 : x; }
inline int toInt(double x) { return int(pow(clamp(x), 1 / 2.2) * 255 + .5); }

inline double cos2sin(double cosValue) {
	//assert(cosValue >= 0);
	return sqrt(1 - cosValue * cosValue);
}
inline double sin2cos(double sinValue) {
	//assert(sinValue >= 0);
	return sqrt(1 - sinValue * sinValue);
}

inline double SampleRandomNumOverride(double start, double end, UINT seed) {
	//std::srand(seed*(unsigned)time(0));
	return (double)rand() / RAND_MAX * (end - start) + start;
}

inline double SampleRandomNum(double start, double end) {
	//std::srand((unsigned)time(0));
	return (double)rand() / RAND_MAX * (end - start) + start;
}
/*
inline Vec3<double> SampleOnSemiSphere(const Vec3<double>& normal) {
	//std::srand((unsigned)time(0));
	//double phi = (double)rand() / RAND_MAX * PI / 2;

	//std::srand((unsigned)time(0));
	//double theta = (double)rand() / RAND_MAX * PI * 2;

	double r1 = 2 * PI * SampleRandomNum(0, 1.0f);
	double r2 = SampleRandomNum(0, 1.0f);
	double r2s = sqrt(r2);
	Vec3<double> w = normal;
	Vec3<double> u = ( (fabs(w.x) > .1 ? Vec3<double>(0, 1, 0) : Vec3<double>(1, 0, 0)).OuterProd( w )  ).normalize();
	auto v = w.OuterProd(u);
	Vec3<double> d = (cos(r1) * r2s * u + sin(r1) * r2s * v + sqrt(1 - r2) * w).normalize();

	return d;
}
*/
inline void resetRandomGen() {
	std::srand((unsigned)time(0));
}


inline Vec3<double> SampleOnSemiSphere(const Vec3<double>& normal) {
	int index = 0;
	while (true) {
		index++;
		double phi = SampleRandomNum(0.0, PI);
		double theta = SampleRandomNum(0.0, 2 * PI);
		Vec3<double> d(sin(phi)*cos(theta), sin(phi)*sin(theta),cos(phi));
		if (d.dot(normal) >= 0)
		return d;
	}


}
inline Vec3<double> SampleScatterDir() {
	double phi = SampleRandomNum(0.0, PI);
	double theta = SampleRandomNum(0.0, 2 * PI);
	Vec3<double> d(sin(phi) * cos(theta), sin(phi) * sin(theta), cos(phi));
	return d;
}

enum MatType {
	Matt,
	Specular,
	Glass,
	Emission
};

struct Photon {
	Vec3<double> position;
	Vec3<double> spectrum;
	Vec3<double> direction;

};


inline void LoadMeshFromFile(const std::string& path, std::vector<Vec3<size_t>>& face, std::vector<Point>& vert, std::vector<Point>& norms) {


	// read
	happly::PLYData plyIn(path);
	std::vector<std::array<double, 3>> vPos = plyIn.getVertexPositions();
	std::vector<std::vector<size_t>> fInd = plyIn.getFaceIndices<size_t>();


	// copy data
	face.resize(fInd.size());
	vert.resize(vPos.size());
	
	memcpy(&vert[0], &vPos[0], vPos.size() * 3 * sizeof(double));
	for (size_t i = 0; i < fInd.size(); i++) {
		face[i].x = fInd[i][0];
		face[i].y = fInd[i][1];
		face[i].z = fInd[i][2];
	}

	// compute normal
	norms.resize(face.size());
	
	for (size_t i = 0; i < face.size(); i++) {
		Point& p1 = vert[face[i].x];
		Point& p2 = vert[face[i].y];
		Point& p3 = vert[face[i].z];

		norms[i] = (p2 - p1).OuterProd(p3 - p1).normalize();
	}
}

// matrix