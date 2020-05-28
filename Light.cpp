#include "Light.h"


Light::Light() : RenderableObject(){
}

Light::Light(std::shared_ptr<Material const> mat, std::shared_ptr<Geometry const> geo) : RenderableObject(mat, geo) {

}