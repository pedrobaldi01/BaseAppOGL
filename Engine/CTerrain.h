#pragma once
#ifdef _MSC_VER
#pragma warning(disable : 4018)
#endif

#include <GL/glew.h>
#include <Engine/stb_image.h>
#include <iostream>
#include <vector>


class CTerrain
{
public:

	CTerrain(const char* filename);
	~CTerrain();

	void DrawTerrain();


private:

	int numStrips;
	int numTrisPerStrip;

	GLuint terrainVAO, terrainVBO, terrainEBO;
};

