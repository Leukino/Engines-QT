#include "Application.h"
#include "ModuleRenderer3D.h"
#include "ModuleImport.h"
//#include "stb_image.h"

#include "ModuleEngine.h"
#include "GameObject.h"
#include "ComponentMesh.h"
#include "ComponentTexture.h"
 
//assimp
#include "Assimp/include/mesh.h"
#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

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

GLuint Import::TextureImport::textureToGL(const char* path) //<-image texture path
{
	GLuint textureID;
	int height;
	int width;
	int compPerPixel;
	unsigned char* pixels = stbi_load(path, &width, &height, &compPerPixel, STBI_rgb);
	LOG("%lf", pixels);
	GLint internalFormat = GL_RGBA;
	if (compPerPixel == 3) internalFormat = GL_RGB;

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, internalFormat, GL_UNSIGNED_BYTE, pixels);

	stbi_image_free(pixels);//DONT NEEDED MORE

	return textureID;

} 

void Import::MeshImport::MeshImport(const char* file) {


	const aiScene* scene = aiImportFile(file, aiProcessPreset_TargetRealtime_MaxQuality);

	if (scene != nullptr && scene->HasMeshes())
	{
		string tempName = "Game Object "; 
		string stringSize = to_string(App->engine->game_objects.size());
		tempName += stringSize;
		GameObject* tempGameObj = new GameObject(tempName);

		// Use scene->mNumMeshes to iterate on scene->mMeshes array
		for (int i = 0; i < scene->mNumMeshes; i++)
		{
			Mesh* tempMesh = new Mesh();
			std::string tmpString = "";
			tmpString.append("New_Obj ");

			ComponentMesh* tempComponentMesh = new ComponentMesh(tempGameObj);

			tempMesh->sizeOf[Mesh::VERTEX] = scene->mMeshes[i]->mNumVertices;
			tempMesh->vertices = new float[tempMesh->sizeOf[Mesh::VERTEX] * 3];
			memcpy(tempMesh->vertices, scene->mMeshes[i]->mVertices, sizeof(float) * tempMesh->sizeOf[Mesh::VERTEX] * 3);
			LOG("New mesh with %d vertices", tempMesh->sizeOf[Mesh::VERTEX]);

			if (scene->mMeshes[i]->HasFaces())
			{
				tempMesh->sizeOf[Mesh::INDEX] = scene->mMeshes[i]->mNumFaces * 3;
				tempMesh->indices = new uint[tempMesh->sizeOf[Mesh::INDEX]];
				for (uint f = 0; f < scene->mMeshes[i]->mNumFaces; ++f)
				{
					if (scene->mMeshes[i]->mFaces[f].mNumIndices != 3)
					{
						LOG("WARNING, geometery face with != 3 indices!");
					}
					else
					{
						memcpy(&tempMesh->indices[f * 3], scene->mMeshes[i]->mFaces[f].mIndices, 3 * sizeof(uint));
					}
				}
				LOG("With %d indices", tempMesh->sizeOf[Mesh::INDEX]);
			}

			if (scene->mMeshes[i]->HasNormals())
			{
				tempMesh->sizeOf[Mesh::NORMAL] = scene->mMeshes[i]->mNumVertices;
				tempMesh->normals = new float[tempMesh->sizeOf[Mesh::NORMAL] * 3];
				memcpy(tempMesh->normals, scene->mMeshes[i]->mNormals, sizeof(float) * tempMesh->sizeOf[Mesh::NORMAL] * 3);
			}

			if (scene->mMeshes[i]->HasTextureCoords(0))
			{
				tempMesh->sizeOf[Mesh::TEXTURE] = scene->mMeshes[i]->mNumVertices;
				tempMesh->texCoords = new float[scene->mMeshes[i]->mNumVertices * 2];

				for (int j = 0; j < tempMesh->sizeOf[Mesh::TEXTURE]; j++)
				{
					tempMesh->texCoords[j * 2] = scene->mMeshes[i]->mTextureCoords[0][j].x;
					tempMesh->texCoords[j * 2 + 1] = scene->mMeshes[i]->mTextureCoords[0][j].y;
				}
			}
			tempComponentMesh->SetMesh(tempMesh);
			tempComponentMesh->SetPath(file);

			tempGameObj->AddComponent(tempComponentMesh);



			App->renderer3D->SetUpBuffers(tempMesh);
			importedMeshes.push_back(tempMesh);
		}

		App->engine->AddGameObject(tempGameObj);
		aiReleaseImport(scene);
	}
	else
	{
		LOG("Error loading scene %s", file);
	}
}