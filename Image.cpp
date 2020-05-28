#include "Image.h"


Image::Image() {
	_img = new std::vector<Color>(0);
}
Image::Image(UINT _w, UINT _h) {
	w = _w;
	h = _h;
	_img = new std::vector<Color>(w*h);
}

bool Image::SetColor(UINT i, UINT j, const Color& c) {
	if (i >= w || j >= h) {  // Խ��
		return false;
	}
	else {
		(*_img)[j*w+i] = c;
		return true;
	}
}

bool Image::Save(const std::string& path) {
	FILE* f = fopen(path.c_str(), "w");         // Write image to PPM file. 
	fprintf(f, "P3\n%d %d\n%d\n", w, h, 255);
	for (int i = 0; i < w * h; i++)
		fprintf(f, "%d %d %d ", toInt((*_img)[i].x), toInt((*_img)[i].y), toInt((*_img)[i].z));
	return true;
}