#pragma once

#include <string>
#include <vector>
#include "utility.h"
class Image
{
private:
	// �洢�ṹ
	std::vector<Color>* _img;
	UINT w, h;
public:
	Image();
	Image(UINT w, UINT h);

	bool SetColor(UINT i, UINT j, const Color& c);

	bool Save(const std::string& path);

};

