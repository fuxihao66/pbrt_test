#pragma once
#include "RenderableObject.h"

class Light : public RenderableObject
{
public:
	Light();
	Light(std::shared_ptr<Material const> mat, std::shared_ptr<Geometry const> geo);
};

