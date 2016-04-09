#include "../Header/simpleraytracer.hpp"

SimpleRayTracer::SimpleRayTracer(unsigned int MaxDepth) {
	this->m_MaxDepth = MaxDepth;
}

void SimpleRayTracer::traceScene(const Scene& SceneModel, RGBImage& Image) {
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
            if(scomp < s){
                //c = tri.pMtrl->getDiffuseCoeff(Vector (0,0,0));
                closest = tri;
                //c = localIllumination(o+d*scomp, o, tri.calcNormal(o+d*scomp), SceneModel.getLight(0), *tri.pMtrl);

                s = scomp;
            }
		}
	}
    Vector surfacePoint = o+d*s;
    if( closest != NULL){
        for(int i=0; i < SceneModel.getLightCount(); i++){
			//sichtverbindung
			//Vector spToLight = SceneModel.getLight(i).Position - surfacePoint;
			//for mit (surfacePoint.triangleIntersection(spToLight, closest->A,closest->B,closest->C,s) == false )
				c += localIllumination(surfacePoint, o, closest->calcNormal(surfacePoint), SceneModel.getLight(i), *(closest->pMtrl));
        }
        
        if(depth > 1){
			Vector sp = surfacePoint;
			Vector recD = ((d*s).reflection(closest->calcNormal(surfacePoint))).normalize();
            c += trace(SceneModel, sp, recD , depth-1) * closest->pMtrl->getReflectivity(sp);
        }
    }
    
    return c;
}

Color SimpleRayTracer::localIllumination(const Vector& SurfacePoint, const Vector& Eye, const Vector& Normal, const PointLight& pointLight, const Material& Material) {
    Color c(0,0,0);
    //diffCo  std::max<float>(0.0, std::min<float>(1.0, f));
    //c+= Material.getAmbientCoeff(SurfacePoint);
    
    Vector lightDir = ( pointLight.Position - SurfacePoint ).normalize();
    c += Material.getDiffuseCoeff(SurfacePoint) * pointLight.Intensity * std::max<float>(0, Normal.dot(lightDir));
    
    //Specular
    Vector e = (Eye - SurfacePoint).normalize();
    Vector r = (-lightDir).reflection(Normal);
    
    c+= Material.getSpecularCoeff(SurfacePoint) * pointLight.Intensity * std::pow(std::max<float>(0, e.dot(r)), Material.getSpecularExp(SurfacePoint) );
    
    
    
	return c;
}