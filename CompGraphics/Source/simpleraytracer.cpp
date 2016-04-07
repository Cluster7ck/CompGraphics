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
	const Triangle* tri;
    const Triangle* closest = NULL;
    float s = FLT_MAX;
    Color c(0,0,0);
	for (unsigned int i = 0; i < SceneModel.getTriangleCount(); i++) {
		tri = &(SceneModel.getTriangle(i));
		
        float scomp = s;
		if (o.triangleIntersection(d, tri->A, tri->B, tri->C, scomp)) {
			//TODO
            if(scomp < s){
                //c = tri.pMtrl->getDiffuseCoeff(Vector (0,0,0));
                closest = tri;
                //c = localIllumination(o+d*scomp, o, tri.calcNormal(o+d*scomp), SceneModel.getLight(0), *tri.pMtrl);

                s = scomp;
            }
		}
	}
    Vector surfacePoint = o+d*s;
    if(closest != NULL){
       c = localIllumination(surfacePoint, o, closest->calcNormal(surfacePoint), SceneModel.getLight(0), *(closest->pMtrl));
    }
    
    return c;
}

Color SimpleRayTracer::localIllumination(const Vector& SurfacePoint, const Vector& Eye, const Vector& Normal, const PointLight&, const Material& Material) {
	//TODO
	return Material.getDiffuseCoeff(SurfacePoint);
}