#include "Mesh.h"


Mesh::Mesh() {
	numVert = 0;
	numFace = 0;
}
Mesh::Mesh(const std::string& path) {
	LoadMeshFromFile(path, faceIndices, vertices);
	numVert = faceIndices.size();
	numFace = vertices.size();
}

const Vec3<double> Mesh::GetNormal(const Vec3<double>& pos) const {

}
bool Mesh::Intersect(const Ray&, double&) const {

}
bool Mesh::Contain(const Vec3<double>& point) const {

}