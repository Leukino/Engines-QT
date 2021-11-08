#ifndef __ModuleImport_H__
#define __ModuleImport_H__

#include "Globals.h"
#include <vector>
#include "OpenGL.h"

using namespace std;



struct Mesh {

	enum Buffer {
		INDEX, VERTEX, NORMAL, TEXTURE, none
	};

	uint* indices = nullptr;
	float* vertices = nullptr;
	float* normals = nullptr;
	float* texCoords = nullptr;

	uint ID[none];
	uint sizeOf[none];
	
	
};

struct Texture {
	GLuint id;
	const char* path;
};

namespace Import {

	namespace MeshImport {
		//Assimp
		//void MeshImport();
		//vector<Mesh*> importedMeshes;
	}
	namespace TextureImport {
		//Devil
		//uint TextureImport(char* file);
		void InitializeDevil();
		//Texture importedTexture;
	}
}

#endif //__ModuleImport_H__
