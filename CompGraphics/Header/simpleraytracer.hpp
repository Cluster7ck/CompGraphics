#ifndef simpleraytracer_hpp
#define simpleraytracer_hpp

#include <stdio.h>
#include "color.hpp"
#include "rgbimage.hpp"
#include "vector.hpp"
#include "CGUtilities.h"
#include "camera.hpp"

class SimpleRayTracer {
public:
	SimpleRayTracer(unsigned int MaxDepth);
	void traceScene(const Scene& SceneModel, RGBImage& Image);

protected:
	Color trace(const Scene& SceneModel, const Vector& o, const Vector& d, int depth);
	Color localIllumination(const Vector& SurfacePoint, const Vector& Eye, const Vector& Normal, const PointLight&, const Material& Material);
	int m_MaxDepth;

};

#endif /* rgbimage_hpp */