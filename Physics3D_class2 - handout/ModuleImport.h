#ifndef __ModuleImporter_H__
#define __ModuleImporter_H__

#include "Globals.h"
#include <vector>
#include "OpenGL.h"


#endif //__ModuleImporter_H__

using namespace std;



struct Mesh {
	uint* indices = nullptr;
	float* vertices = nullptr;
	float* normals = nullptr;
	float* texCoords = nullptr;

	uint ID[4] = {0, 0, 0, 0};
	uint sizeOf[4] = {0, 0, 0, 0};
	
	enum 
};

struct Texture {
	GLuint id;
	const char* path;
};

//Assimp
void AssimpImport();

//Devil
uint DevilImport(char* file);
void InitDevil();


vector<Mesh*> importedMeshes;
Texture importedTexture;