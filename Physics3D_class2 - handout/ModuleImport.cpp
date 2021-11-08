#include "Application.h"
#include "ModuleRenderer3D.h"
#include "ModuleImport.h"

//assimp
#include "Assimp/include/mesh.h"
#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
//devil
#include "Devil/include/ilut.h"
#include "Devil/include/ilu.h"
#include "Devil/include/il.h"

#pragma comment (lib, "Assimp/libx86/assimp.lib")
#pragma comment (lib, "Devil/libx86/devIL.lib")
#pragma comment (lib, "Devil/libx86/ILU.lib")
#pragma comment (lib, "Devil/libx86/ILUT.lib")


void Import::TextureImport::InitializeDevil() {
	ilInit();
	iluInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);
	LOG("Initialized DevIL");
}

GLuint LoadVertexToGL(aiVector3D* vertices, size_t numVertices) {

	const GLint numberOfCoordPerVertex = 3;
	const size_t arraySize = numVertices * numberOfCoordPerVertex * sizeof(float);

	GLuint verticesID;
	glGenBuffers(1, &verticesID);
	glAccum(GL_ARRAY_BUFFER, verticesID);
	glBufferData(GL_ARRAY_BUFFER, arraySize, vertices, GL_STATIC_DRAW);

	return verticesID;
}

GLuint LoadIndicesToGL(aiFace* faces, size_t numFaces) {
	const GLint numberIndexPerFace = 3;
	const GLsizei numberOfIndex = (GLsizei)(numFaces * numberIndexPerFace);
	const size_t arrSize = numberOfIndex * sizeof(int);
	unsigned int* indicesArray = (unsigned int*)malloc(arrSize);

	for (int i = 0; i < numFaces; i++)
	{
		aiFace* f = faces + i;
		*(indicesArray + 0 + i * 3) = f->mIndices[0];
		*(indicesArray + 1 + i * 3) = f->mIndices[0];
		*(indicesArray + 2 + i * 3) = f->mIndices[0];
	}

	GLuint indicesID;
	glGenBuffers(1, &indicesID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, arrSize, indicesArray, GL_STATIC_DRAW);

	free(indicesArray);

	return indicesID;
}

GLuint LoadNormalsToGL(aiVector3D* normals, size_t numNormals)
{
	const GLint numberOfCoordPerVertex = 3;
	const size_t  arrSize = numNormals * numberOfCoordPerVertex * sizeof(float);

	GLuint normalsID;
	glGenBuffers(1, &normalsID);
	glBindBuffer(GL_ARRAY_BUFFER, normalsID);
	glBufferData(GL_ARRAY_BUFFER, arrSize, normals, GL_STATIC_DRAW);

	return normalsID;
}

GLuint textCoordToGL(aiVector3D* coord, size_t numTextCoor)
{
	const GLint     numberOfComponents = 2; //2d u,v
	const size_t    textCoordArraySizeinBytes = numTextCoor * sizeof(float) * numberOfComponents;



	//because the librery uses aiVector3D, and we need only u,v
	float* textCoordArray = (float*)malloc(textCoordArraySizeinBytes);
	for (int i = 0; i < numTextCoor; i++)
	{
		*(textCoordArray + i * 2) = coord[i].x;
		*(textCoordArray + i * 2 + 1) = 1.0 - coord[i].y; //this coord image is inverted in the library
	}

	GLuint textCoordID;
	glGenBuffers(1, &textCoordID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, textCoordID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, textCoordArraySizeinBytes, textCoordArray, GL_STATIC_DRAW);

	free(textCoordArray); //DONT NEEDED MORE

	return textCoordID;
}
