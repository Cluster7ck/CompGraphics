//
//  terrain.cpp
//  CG_P1
//
//  Created by  on 25.05.16.
//  Copyright � 2016 hsos. All rights reserved.
//

#include "../Header/terrain.h"

#define BUFFER_OFFSET(i) ((char *)NULL + (i))
Vector triangleNormal(Vector a, Vector b, Vector c);

Terrain::Terrain() {

}

Terrain::~Terrain() {

}

bool Terrain::load(const char* HeightMap, const char* DetailMap1, const char* DetailMap2, const char* MixMap, float Width, float Depth, float HeightMultiplier) {
	static Texture imgTexture;

	if (!imgTexture.LoadFromBMP(HeightMap)) {
		std::cout << "Fehler beim Lesen der HeightMap-Datei." << std::endl;
		return false;
	}

	// Aufgabe 2
	/*if (!m_GrassTex.LoadFromBMP(DetailMap1)) {
		std::cout << "Fehler beim Lesen der DetailMap1-Datei." << std::endl;
		return false;
	}

	if (!m_SandTex.LoadFromBMP(DetailMap2)) {
		std::cout << "Fehler beim Lesen der DetailMap2-Datei." << std::endl;
		return false;
	}

	if (!m_MixingRatio.LoadFromBMP(MixMap)) {
		std::cout << "Fehler beim Lesen der MixMap-Datei." << std::endl;
		return false;
	}*/

	RGBImage img = *imgTexture.getRGBImage();
	int imgWidth = img.width();
	int imgHeight = img.height();

	// Vertices
	TerrainVertex* Vertices = new TerrainVertex[imgWidth*imgHeight];

	for (int x = 0; x < imgWidth; x++) {
		for (int y = 0; y < imgHeight; y++) {
			Color currentColor = img.getPixelColor(x, y);
			Vertices[x * imgWidth + y].Pos.X = x/(imgWidth*1.0f)*Width - (Width / 2);
			Vertices[x * imgWidth + y].Pos.Z = y/(imgHeight*1.0f)*Depth - (Depth / 2);
			Vertices[x * imgHeight + y].Pos.Y = currentColor.R/255 * HeightMultiplier;
			Vertices[x * imgHeight + y].Normal = Vector();
			//F�r Mixmap
			Vertices[x * imgHeight + y].u0 = 0;
			Vertices[x * imgHeight + y].v0 = 0;
			//F�r Detailsmap
			Vertices[x * imgHeight + y].u1 = 0;
			Vertices[x * imgHeight + y].v1 = 0;
		}
	}

	// Indices
	indicesCount = (imgWidth * imgHeight) * 6;
	unsigned int *Indices = new unsigned int[indicesCount];
	unsigned int vertexIndex = 0;

	for (int x = 0; x < imgWidth; x++) {
		for (int y = 0; y < imgHeight; y++) {
			if (x < imgWidth - 1 && y < imgHeight - 1) {
				Indices[vertexIndex] = x * imgWidth + y;
				Indices[vertexIndex + 1] = (x + 1)* imgWidth + (y + 1);
				Indices[vertexIndex + 2] = (x + 1)* imgWidth + y;
				
				Indices[vertexIndex + 3] = (x)* imgWidth + y;
				Indices[vertexIndex + 4] = (x)* imgWidth + (y + 1);
				Indices[vertexIndex + 5] = (x + 1)* imgWidth + (y + 1);
				vertexIndex += 6;
			}
		}
	}

	// Calc normals
	for (int x = 0; x < imgWidth; x++) {
		for (int y = 0; y < imgHeight; y++) {
			Vector a, b, c;
			Vector vertexNormal, normalTri1, normalTri2, normalTri3, normalTri4, normalTri5, normalTri6;
			
			if(x == 0 && y == 0){
				a = Vertices[x * imgWidth + y].Pos;
				b = Vertices[(x + 1)* imgWidth + y].Pos;
				c = Vertices[(x + 1)* imgWidth + (y + 1)].Pos;
				normalTri1 = triangleNormal(a,b,c);

				a = Vertices[x * imgWidth + y].Pos;
				b = Vertices[(x + 1)* imgWidth + (y + 1)].Pos;
				c = Vertices[x * imgWidth + (y + 1)].Pos;
				normalTri2 = triangleNormal(a, b, c);
				
				vertexNormal = ((normalTri1 + normalTri2) * .5f).normalize();
			}
			else if (y == 0) {
				if (x == imgWidth - 1) {
					a = Vertices[x * imgWidth + y].Pos;
					b = Vertices[x * imgWidth + (y + 1)].Pos;
					c = Vertices[(x - 1)* imgWidth + y].Pos;
					vertexNormal = triangleNormal(a, b, c);
				}
				else {
					a = Vertices[x * imgWidth + y].Pos;
					b = Vertices[(x + 1)* imgWidth + (y + 1)].Pos;
					c = Vertices[x * imgWidth + (y + 1)].Pos;
					normalTri1 = triangleNormal(a, b, c);

					a = Vertices[x * imgWidth + y].Pos;
					b = Vertices[(x + 1)* imgWidth + y].Pos;
					c = Vertices[(x + 1)* imgWidth + (y + 1)].Pos;
					normalTri2 = triangleNormal(a, b, c);

					a = Vertices[x * imgWidth + y].Pos;
					b = Vertices[x * imgWidth + (y + 1)].Pos;
					c = Vertices[(x-1) * imgWidth + y].Pos;
					normalTri3 = triangleNormal(a, b, c);

					vertexNormal = ((normalTri1 + normalTri2 + normalTri3) * (1/3.0f)).normalize();
				}
			}
			else if (x == 0) {
				if (y == imgHeight - 1) {
					a = Vertices[x * imgWidth + (y)].Pos;
					b = Vertices[x * imgWidth + (y-1)].Pos;
					c = Vertices[(x + 1)* imgWidth + y].Pos;
					vertexNormal = triangleNormal(a, b, c);
				}
				else{
					a = Vertices[x * imgWidth + y].Pos;
					b = Vertices[x * imgWidth + (y-1)].Pos;
					c = Vertices[(x + 1)* imgWidth + y].Pos;
					normalTri1 = triangleNormal(a, b, c);

					a = Vertices[x * imgWidth + y].Pos;
					b = Vertices[(x + 1)* imgWidth + y].Pos;
					c = Vertices[(x + 1)* imgWidth + (y + 1)].Pos;
					normalTri2 = triangleNormal(a, b, c);

					a = Vertices[x * imgWidth + y].Pos;
					b = Vertices[(x + 1)* imgWidth + (y + 1)].Pos;
					c = Vertices[x * imgWidth + (y + 1)].Pos;
					normalTri3 = triangleNormal(a, b, c);

					vertexNormal = ((normalTri1 + normalTri2 + normalTri3) * (1 / 3.0f)).normalize();
				}
			}
			else if (x == imgWidth - 1) {
				if (y == imgHeight - 1) {
					a = Vertices[x* imgWidth + y].Pos;
					b = Vertices[(x-1) * imgWidth + y].Pos;
					c = Vertices[(x - 1)* imgWidth + (y-1)].Pos;
					normalTri1 = triangleNormal(a, b, c);

					a = Vertices[x* imgWidth + y].Pos;
					b = Vertices[(x-1) * imgWidth + (y - 1)].Pos;
					c = Vertices[x * imgWidth + (y-1)].Pos;
					normalTri2 = triangleNormal(a, b, c);

					vertexNormal = ((normalTri1 + normalTri2) * .5f).normalize();
				}
				else {
					a = Vertices[x* imgWidth + y].Pos;
					b = Vertices[(x - 1) * imgWidth + y].Pos;
					c = Vertices[(x - 1)* imgWidth + (y - 1)].Pos;
					normalTri1 = triangleNormal(a, b, c);

					a = Vertices[x* imgWidth + y].Pos;
					b = Vertices[(x - 1) * imgWidth + (y - 1)].Pos;
					c = Vertices[x * imgWidth + (y - 1)].Pos;
					normalTri2 = triangleNormal(a, b, c);

					a = Vertices[x* imgWidth + y].Pos;
					b = Vertices[x * imgWidth + (y+1)].Pos;
					c = Vertices[(x-1) * imgWidth + y].Pos;
					normalTri3 = triangleNormal(a, b, c);

					vertexNormal = ((normalTri1 + normalTri2 + normalTri3) * (1 / 3.0f)).normalize();
				}
			}
			else if(x < imgWidth - 1 && y < imgHeight - 1) {
				a = Vertices[x* imgWidth + y].Pos;
				b = Vertices[(x - 1) * imgWidth + y].Pos;
				c = Vertices[(x - 1)* imgWidth + (y - 1)].Pos;
				normalTri1 = triangleNormal(a, b, c);

				a = Vertices[x* imgWidth + y].Pos;
				b = Vertices[(x - 1) * imgWidth + (y - 1)].Pos;
				c = Vertices[x * imgWidth + (y - 1)].Pos;
				normalTri2 = triangleNormal(a, b, c);

				a = Vertices[x* imgWidth + y].Pos;
				b = Vertices[x * imgWidth + (y + 1)].Pos;
				c = Vertices[(x - 1) * imgWidth + y].Pos;
				normalTri3 = triangleNormal(a, b, c);
				
				a = Vertices[x * imgWidth + y].Pos;
				b = Vertices[x * imgWidth + (y - 1)].Pos;
				c = Vertices[(x + 1)* imgWidth + y].Pos;
				normalTri4 = triangleNormal(a, b, c);

				a = Vertices[x * imgWidth + y].Pos;
				b = Vertices[(x + 1)* imgWidth + y].Pos;
				c = Vertices[(x + 1)* imgWidth + (y + 1)].Pos;
				normalTri5 = triangleNormal(a, b, c);

				a = Vertices[x * imgWidth + y].Pos;
				b = Vertices[(x + 1)* imgWidth + (y + 1)].Pos;
				c = Vertices[x * imgWidth + (y + 1)].Pos;
				normalTri6 = triangleNormal(a, b, c);

				vertexNormal = ((normalTri1 + normalTri2 + normalTri3 + normalTri4 + normalTri5 + normalTri6) * (1 / 6.0f)).normalize();
			}

			Vertices[x * imgWidth + y].Normal = vertexNormal * -1;
		}
	}

	// create gpu buffer for vertices
	glGenBuffers(1, &m_VertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(TerrainVertex) * (imgWidth * imgHeight), Vertices, GL_STATIC_DRAW);

	// create gpu buffer for indices
	glGenBuffers(1, &m_IndexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indicesCount, Indices, GL_STATIC_DRAW);

	return true;
}

void Terrain::draw() {
	// inform the client that we want to use array buffers
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	// setup position & normal pointers
	glVertexPointer(3, GL_FLOAT, sizeof(TerrainVertex), BUFFER_OFFSET(0));
	glNormalPointer(GL_FLOAT, sizeof(TerrainVertex), BUFFER_OFFSET(12));

	// setup texture-environment-unit 0 => grass
	glActiveTexture(GL_TEXTURE0);
	glClientActiveTexture(GL_TEXTURE0);
	glTexCoordPointer(2, GL_FLOAT, sizeof(TerrainVertex), BUFFER_OFFSET(24)); // first uv-set
	m_GrassTex.apply();
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	// setup texture-environment-unit 1 => sand
	glActiveTexture(GL_TEXTURE1);
	glClientActiveTexture(GL_TEXTURE1);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, sizeof(TerrainVertex), BUFFER_OFFSET(32)); // second uv-set
	m_SandTex.apply();
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	// setup texture-environment-unit 2 => grass*sand
	glActiveTexture(GL_TEXTURE2);
	glClientActiveTexture(GL_TEXTURE2);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, sizeof(TerrainVertex), BUFFER_OFFSET(24)); // is obsolete, but we have to supply it
	m_MixingRatio.apply(); 
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
	glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_MODULATE);
	glTexEnvf(GL_TEXTURE_ENV, GL_SRC0_RGB, GL_TEXTURE0); // arg0 is texture from unit 0
	glTexEnvf(GL_TEXTURE_ENV, GL_SRC1_RGB, GL_TEXTURE1); // arg1 is texture from unit 1
	glTexEnvf(GL_TEXTURE_ENV, GL_SRC2_RGB, GL_TEXTURE2); // arg2 is texture from unit 2
	glTexEnvf(GL_TEXTURE_ENV, GL_OPERAND0_RGB, GL_SRC_COLOR);
	glTexEnvf(GL_TEXTURE_ENV, GL_OPERAND1_RGB, GL_SRC_COLOR);
	glTexEnvf(GL_TEXTURE_ENV, GL_OPERAND2_RGB, GL_SRC_COLOR);

	// we draw our terrain
	glDrawElements(GL_TRIANGLES, indicesCount, GL_UNSIGNED_INT, BUFFER_OFFSET(0));

	// disable states in reverse order
	glDisable(GL_TEXTURE_2D);

	glActiveTexture(GL_TEXTURE1);
	glClientActiveTexture(GL_TEXTURE1);
	glDisable(GL_TEXTURE_2D);

	glActiveTexture(GL_TEXTURE0);
	glClientActiveTexture(GL_TEXTURE0);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); // set modulate as default behaviour for unit 0
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

Vector triangleNormal(Vector a, Vector b, Vector c) {
	return (b - a).cross(c - a);
	//return (b - c).cross(a - c);
	//return (c - a).cross(b - a);
}