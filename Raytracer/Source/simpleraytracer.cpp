#include "../Header/simpleraytracer.hpp"

#define EPSILON 1e-3

SimpleRayTracer::SimpleRayTracer(unsigned int MaxDepth) {
	this->m_MaxDepth = MaxDepth;
}

void SimpleRayTracer::traceScene(const Scene& SceneModel, RGBImage& Image) {
	Camera eye(-8.0f, 1.0f, 1.0f, 0.75f, 640.0f, 480.0f);

	for (unsigned int x = 0; x < Image.width(); x++) {
		for (unsigned int y = 0; y < Image.height(); y++) {
			//Farbe f�r Pixel berechnen
			Image.setPixelColor(x, y, trace(SceneModel, eye.Position(), eye.generateRay(x, y), m_MaxDepth));
		}
	}
}

Color SimpleRayTracer::trace(const Scene& SceneModel, const Vector& o, const Vector& d, int depth) {
	const Triangle* tri;
    const Triangle* closest = NULL;
    float s = FLT_MAX;
    Color c;

	for (unsigned int i = 0; i < SceneModel.getTriangleCount(); i++) {
		tri = &(SceneModel.getTriangle(i));
		
        float scomp;		//Strahl, wird in triangleIntersection bearbeitet
		if (o.triangleIntersection(d, tri->A, tri->B, tri->C, scomp) && scomp < s && scomp > EPSILON){
            closest = tri;
			s = scomp;
		}
	}

	if (closest != NULL) {
		Vector surfacePoint = o + d * s;
		//normale vom Dreieck am Schnittpunkt
		Vector normalSurfacePoint = closest->calcNormal(surfacePoint);
		
		for (int i = 0; i < SceneModel.getLightCount(); i++) {
			bool intersects = false;

			PointLight pointLight = SceneModel.getLight(i);
			//Vec surface to light
			Vector lightDir = pointLight.Position - surfacePoint;
			float lightDist = lightDir.length();
			
			//Auskommentieren, wenn kein Schatten gew�nscht von
			for (int j = 0; j < SceneModel.getTriangleCount(); j++) {
				float s_light;
				Triangle current = SceneModel.getTriangle(j);
				
				//Sichtbares Licht
				if (surfacePoint.triangleIntersection(lightDir.normalize(), current.A, current.B, current.C, s_light) && s_light < lightDist && s_light > EPSILON) {
					intersects = true;
				}
			}
			//Auskommentieren, wenn kein Schatten gew�nscht bis
			
			//Sichtverbindung
			if (!intersects) {
				c += localIllumination(surfacePoint, o, normalSurfacePoint, pointLight, *closest->pMtrl);
			}
		}

		//Reflektion
		if (depth > 0) {
			Vector recD = d.reflection(normalSurfacePoint).normalize();
			c += trace(SceneModel, surfacePoint, recD, depth - 1) * closest->pMtrl->getReflectivity(surfacePoint);
		}
		//Transmission
	}

    return c;
}

Color SimpleRayTracer::localIllumination(const Vector& SurfacePoint, const Vector& Eye, const Vector& Normal, const PointLight& pointLight, const Material& Material) {
    Color c;
    
	//Defuse
    Vector lightDir = ( pointLight.Position - SurfacePoint ).normalize();
    c += Material.getDiffuseCoeff(SurfacePoint) * pointLight.Intensity * std::max<float>(0, Normal.dot(lightDir));
    
    //Specular
    Vector e = (Eye - SurfacePoint).normalize();
    Vector r = (-lightDir).reflection(Normal).normalize();
    
    c += Material.getSpecularCoeff(SurfacePoint) * pointLight.Intensity * std::pow(std::max<float>(0, e.dot(r)), Material.getSpecularExp(SurfacePoint) );
    
	//Ambient
	//c += Material.getAmbientCoeff(SurfacePoint);
    
	return c;
}