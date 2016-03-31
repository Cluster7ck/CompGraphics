#include "../Header/simpleraytracer.hpp"

SimpleRayTracer::SimpleRayTracer(unsigned int MaxDepth) {
	this->m_MaxDepth = MaxDepth;
}

void SimpleRayTracer::traceScene(const Scene& SceneModel, RGBImage& Image) {
	
}

Color SimpleRayTracer::trace(const Scene& SceneModel, const Vector& o, const Vector& d, int depth) {
	//TODO
}

Color SimpleRayTracer::localIllumination(const Vector& SurfacePoint, const Vector& Eye, const Vector& Normal, const PointLight&, const Material& Material) {
	//TODO
}