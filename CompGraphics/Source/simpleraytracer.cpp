#include "../Header/simpleraytracer.hpp"

SimpleRayTracer::SimpleRayTracer(unsigned int MaxDepth) {
	this->m_MaxDepth = MaxDepth;
}

void SimpleRayTracer::traceScene(const Scene& SceneModel, RGBImage& Image) {
	//TODO
	Camera eye(-8.0f, 1.0f, 1.0f, 0.75f, 640.0f, 480.0f);

	Color pColor;
	Vector ray;
	
	for (unsigned int y = 0; y < Image.height(); y++) {
		 for (unsigned int x = 0; x < Image.width(); x++) {
			ray = eye.generateRay(x, y);
			
			pColor = trace(SceneModel, eye.Position(), ray, m_MaxDepth);
			Image.setPixelColor(x, y, pColor);
		}
	}
}

Color SimpleRayTracer::trace(const Scene& SceneModel, const Vector& o, const Vector& d, int depth) {
	Triangle tri;
	for (unsigned int i = 0; i < SceneModel.getTriangleCount(); i++) {
		tri = SceneModel.getTriangle(i);
		float s = 0;

		if (o.triangleIntersection(d, tri.A, tri.B, tri.C, s)) {
			//TODO
			//return localIllumination(o+d*s,o,tri.calcNormal(o + d*s),)
			return tri.pMtrl->getDiffuseCoeff(Vector (0,0,0));
		}
	}
	return Color(0, 0, 0);
}

Color SimpleRayTracer::localIllumination(const Vector& SurfacePoint, const Vector& Eye, const Vector& Normal, const PointLight&, const Material& Material) {
	//TODO
	return Material.getDiffuseCoeff(SurfacePoint);
}