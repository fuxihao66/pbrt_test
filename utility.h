#pragma once
#include <cmath>
#include <stdlib.h>
#define M_PI 3.1415926536


struct Vec {        
	double x, y, z;                  
	Vec(double x_ = 0, double y_ = 0, double z_ = 0) { 
		x = x_; 
		y = y_; 
		z = z_; 
	}

	Vec operator+(const Vec& b) const { 
		return Vec(x + b.x, y + b.y, z + b.z); 
	}

	Vec operator-(const Vec& b) const { 
		return Vec(x - b.x, y - b.y, z - b.z); 
	}

	Vec operator*(double b) const { 
		return Vec(x * b, y * b, z * b); 
	}

	Vec mult(const Vec& b) const { 
		return Vec(x * b.x, y * b.y, z * b.z); 
	}

	Vec& norm() { 
		return *this = *this * (1 / sqrt(x * x + y * y + z * z)); 
	}

	double dot(const Vec& b) const { 
		return x * b.x + y * b.y + z * b.z; 
	} 
	// cross: 
	Vec operator%(Vec& b) { 
		return Vec(y * b.z - z * b.y, z * b.x - x * b.z, x * b.y - y * b.x);
	}
};

struct Ray { 
	Vec o, d; 
	Ray(const Vec& o_, const Vec& d_) : o(o_), d(d_) {}
};

enum Refl_t { DIFF, SPEC, REFR };

struct Sphere {
	double rad;       // radius 
	Vec p, e, c;      // position, emission, color 
	Refl_t refl;      // reflection type (DIFFuse, SPECular, REFRactive) 
	Sphere(double rad_, Vec p_, Vec e_, Vec c_, Refl_t refl_) :
		rad(rad_), p(p_), e(e_), c(c_), refl(refl_) {}
	double intersect(const Ray& r) const { // returns distance, 0 if nohit 
		Vec op = p - r.o; // Solve t^2*d.d + 2*t*(o-p).d + (o-p).(o-p)-R^2 = 0 
		double t, eps = 1e-4, b = op.dot(r.d), det = b * b - op.dot(op) + rad * rad;
		if (det < 0) return 0; else det = sqrt(det);
		return (t = b - det) > eps ? t : ((t = b + det) > eps ? t : 0);
	}
};

inline double clamp(double x) { return x < 0 ? 0 : x>1 ? 1 : x; }
inline int toInt(double x) { return int(pow(clamp(x), 1 / 2.2) * 255 + .5); }
inline bool intersect(const std::vector<Sphere>& spheres, const Ray& r, double& t, int& id) {
	double numSph = spheres.size();
	double distance;
	double inf = t = 1e20;
	for (int i = 0; i < numSph;  i++) {                            // find the closest one that intersect happened
		if ((distance = spheres[i].intersect(r)) && distance < t) {
			t = distance;
			id = i; 
		}
	}

	return t < inf;
}

double erand48(unsigned short xsubi[3]) {
	return (double)rand() / (double)RAND_MAX;
}

Vec radiance(const std::vector<Sphere>& spheres, const Ray& r, int depth, unsigned short* Xi) {
	double t;                               // distance to intersection 
	int id = 0;                               // id of intersected object 
	if (!intersect(spheres, r, t, id))
		return Vec(); // if miss, return black 
	const Sphere& obj = spheres[id];        // the hit object 

	Vec x = r.o + r.d * t, n = (x - obj.p).norm();
	Vec nl = n.dot(r.d) < 0 ? n : n * -1;
	Vec f = obj.c;

	double p = f.x > f.y && f.x > f.z ? f.x : f.y > f.z ? f.y : f.z; // max refl 

	if (++depth > 5) 
		if (erand48(Xi) < p) f = f * (1 / p);
	else return obj.e; //R.R. 

	if (obj.refl == DIFF) {                  // Ideal DIFFUSE reflection 
		double r1 = 2 * M_PI * erand48(Xi), r2 = erand48(Xi), r2s = sqrt(r2);
		Vec w = nl, u = ((fabs(w.x) > .1 ? Vec(0, 1) : Vec(1)) % w).norm(), v = w % u;
		Vec d = (u * cos(r1) * r2s + v * sin(r1) * r2s + w * sqrt(1 - r2)).norm();
		return obj.e + f.mult(radiance(spheres, Ray(x, d), depth, Xi));
	}
	else if (obj.refl == SPEC)            // Ideal SPECULAR reflection 
		return obj.e + f.mult(radiance(spheres, Ray(x, r.d - n * 2 * n.dot(r.d)), depth, Xi));


	Ray reflRay(x, r.d - n * 2 * n.dot(r.d));     // Ideal dielectric REFRACTION 
	bool into = n.dot(nl) > 0;                // Ray from outside going in? 
	double nc = 1, nt = 1.5, nnt = into ? nc / nt : nt / nc, ddn = r.d.dot(nl), cos2t;
	if ((cos2t = 1 - nnt * nnt * (1 - ddn * ddn)) < 0)    // Total internal reflection 
		return obj.e + f.mult(radiance(spheres, reflRay, depth, Xi));
	Vec tdir = (r.d * nnt - n * ((into ? 1 : -1) * (ddn * nnt + sqrt(cos2t)))).norm();
	double a = nt - nc, b = nt + nc, R0 = a * a / (b * b), c = 1 - (into ? -ddn : tdir.dot(n));
	double Re = R0 + (1 - R0) * c * c * c * c * c, Tr = 1 - Re, P = .25 + .5 * Re, RP = Re / P, TP = Tr / (1 - P);
	return obj.e + f.mult(depth > 2 ? (erand48(Xi) < P ?   // Russian roulette 
		radiance(spheres, reflRay, depth, Xi) * RP : radiance(spheres, Ray(x, tdir), depth, Xi) * TP) :
		radiance(spheres, reflRay, depth, Xi) * Re + radiance(spheres, Ray(x, tdir), depth, Xi) * Tr);
}