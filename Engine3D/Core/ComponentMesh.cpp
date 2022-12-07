#include "ComponentMesh.h"

#include "glew.h"
#include "SDL/include/SDL_opengl.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ComponentMaterial.h"
#include "ComponentTransform.h"
#include "ComponentMesh.h"
#include "GameObject.h"
#include "ImGui/imgui.h"
#include "Geometry/Sphere.h"
#include "par_shapes.h"
#include <fstream>
#include "MathGeoLib/include/Geometry/Plane.h"
#include "ModuleImport.h"

ComponentMesh::ComponentMesh(GameObject* parent) : Component(parent) {
	this->type = "Mesh";
}

ComponentMesh::ComponentMesh(GameObject* parent, Shape shape) : Component(parent)
{
	this->type = "Mesh";
	switch (shape)
	{
	case Shape::CUBE:
		CopyParMesh(par_shapes_create_cube());		
		break;
	case Shape::CYLINDER:
		CopyParMesh(par_shapes_create_cylinder(20, 20));
		break;
	case Shape::SPHERE:
		CopyParMesh(par_shapes_create_parametric_sphere(20, 20));
		break;
	}
}

ComponentMesh::~ComponentMesh()
{
	vertexBufferId ? glDeleteBuffers(1, &vertexBufferId) : 0;
	textureBufferId ? glDeleteBuffers(1, &textureBufferId) : 0;
	indexBufferId ? glDeleteBuffers(1, &indexBufferId) : 0;
}

void ComponentMesh::CopyParMesh(par_shapes_mesh* parMesh)
{
	numVertices = parMesh->npoints;
	numIndices = parMesh->ntriangles * 3;
	numNormalFaces = parMesh->ntriangles;
	vertices.resize(numVertices);
	normals.resize(numVertices);
	indices.resize(numIndices);
	par_shapes_compute_normals(parMesh);
	for (size_t i = 0; i < numVertices; ++i)
	{
		memcpy(&vertices[i], &parMesh->points[i * 3], sizeof(float) * 3);
		memcpy(&normals[i], &parMesh->normals[i * 3], sizeof(float) * 3);
	}
	for (size_t i = 0; i < indices.size(); ++i)
	{
		indices[i] = parMesh->triangles[i];
	}
	memcpy(&normals[0], parMesh->normals, numVertices);

	par_shapes_free_mesh(parMesh);

	GenerateBuffers();
	ComputeNormals();
	GenerateBounds();
}


void ComponentMesh::GenerateBuffers() {
	
	//-- Generate Vertex
	vertexBufferId = 0;
	glGenBuffers(1, &vertexBufferId);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float3) * numVertices, &vertices[0], GL_STATIC_DRAW);

	//-- Generate Index
	indexBufferId = 0;
	glGenBuffers(1, &indexBufferId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * numIndices, &indices[0], GL_STATIC_DRAW);

	//-- Generate Texture_Buffers
	if (texCoords.size() != 0)
	{
		textureBufferId = 0;
		glGenBuffers(1, &textureBufferId);
		glBindBuffer(GL_ARRAY_BUFFER, textureBufferId);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float2) * texCoords.size(), &texCoords[0], GL_STATIC_DRAW);
	}
	if (vertexBufferId == 0 || indexBufferId == 0)
		LOG("Error creating mesh on gameobject %s", owner->name.c_str());
}

void ComponentMesh::ComputeNormals()
{

	numNormalFaces = numIndices * 3;
	faceNormals.resize(numNormalFaces);
	faceCenters.resize(numNormalFaces);

	for (size_t i = 0; i < numIndices; i += 3)
	{
		const float3 p0 = vertices[indices[i + 1]] - vertices[indices[i]];
		const float3 p1 = vertices[indices[i + 2]] - vertices[indices[i]];

		const float3 faceNormal = float3(p0.x, p0.y, p0.z).Cross(float3(p1.x, p1.y, p1.z));
		faceNormals[i / 3] = faceNormal.Normalized();

		const float3 faceCenter = (vertices[indices[i]] + vertices[indices[i + 1]] + vertices[indices[i + 2]]) / 3.f;
		faceCenters[i / 3] = faceCenter;
	}
	int kk = 0;
}

void ComponentMesh::GenerateBounds()
{
	
	localAABB.SetNegativeInfinity();
	if (vertices.size() > 0)
	localAABB.Enclose(&vertices[0], vertices.size());
	Sphere sphere;	
	float3 position = owner->GetComponent<ComponentTransform>()->GetPosition();
	float3 scale = owner->GetComponent<ComponentTransform>()->GetScale();
	position = float3(position.x * scale.x, position.y * scale.y, position.z * scale.z);
	sphere.r = 0.f * scale.Normalize();
	sphere.pos = position;
	sphere.Enclose(localAABB);
	localAABB.TransformAsAABB(owner->transform->transformMatrix);
	radius = sphere.r;
	centerPoint = localAABB.CenterPoint();
}

void ComponentMesh::DrawNormals() const
{
	if (drawFaceNormals)
	{
		for (size_t i = 0; i < faceNormals.size(); ++i)
		{
			glColor3f(0.f, 0.f, 1.f);
			glBegin(GL_LINES);			
			const float3 faceCenter = owner->transform->transformMatrix.TransformPos(faceCenters[i]);
			const float3 faceNormalPoint = faceCenter + faceNormals[i] * normalScale;
			glVertex3f(faceCenter.x, faceCenter.y, faceCenter.z);
			glVertex3f(faceNormalPoint.x, faceNormalPoint.y, faceNormalPoint.z);
			glEnd();
		}
	}
	if (drawVertexNormals)
	{
		for (size_t i = 0; i < normals.size(); ++i)
		{
			glColor3f(1.f, 0.f, 0.f);
			glBegin(GL_LINES);
			const float3 vertexPos = owner->transform->transformMatrix.TransformPos(vertices[i]);
			const float3 vertexNormalPoint = vertexPos + normals[i] * normalScale;
			glVertex3f(vertexPos.x, vertexPos.y, vertexPos.z);
			glVertex3f(vertexNormalPoint.x, vertexNormalPoint.y, vertexNormalPoint.z);
			glEnd();
		}
	}
}

float3 ComponentMesh::GetCenterPointInWorldCoords() const
{
	return owner->transform->transformMatrix.TransformPos(centerPoint);
}

bool ComponentMesh::Update(float dt)
{

	if (App->pause) 
		return true;

	drawWireframe || App->renderer3D->wireframeMode ? glPolygonMode(GL_FRONT_AND_BACK, GL_LINE) : glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glBindTexture(GL_TEXTURE_2D, 0);

	//--Enable States--//
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	//-- Buffers--//
	glBindBuffer(GL_ARRAY_BUFFER, this->textureBufferId);
	glTexCoordPointer(2, GL_FLOAT, 0, NULL);

	glBindBuffer(GL_ARRAY_BUFFER, this->vertexBufferId);
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	GenerateBounds();

	if (ComponentMaterial* material = owner->GetComponent<ComponentMaterial>())
	{	
		drawWireframe || !App->renderer3D->useTexture || App->renderer3D->wireframeMode ? 0 : glBindTexture(GL_TEXTURE_2D, material->GetTextureId());
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->indexBufferId);
	if (App->camera->cameraFrustum.Contains(centerPoint))
	{
		//-- Draw --//

		glPushMatrix();
		glMultMatrixf(owner->transform->transformMatrix.Transposed().ptr());
		glColor3f(1.0f, 1.0f, 1.0f);
		glDrawElements(GL_TRIANGLES, this->numIndices, GL_UNSIGNED_INT, NULL);
		glPopMatrix();
		App->renderer3D->drawCalls += this->numIndices;
		App->renderer3D->drawnObjects++;
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_TEXTURE_COORD_ARRAY, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	if (App->renderer3D->renderAABB)
	{
		//float3 pos = owner->GetComponent<ComponentTransform>()->GetPosition();
		float4x4 pos = owner->GetComponent<ComponentTransform>()->transformMatrix;
		//float3 point = localAABB.CenterPoint();
		float3 point = localAABB.CenterPoint();
		glColor3f(3.f, 3.f, 3.f);
		if (this->owner->isSelected)
			glColor3f(0.f, 3.f, 0.f);
		glBegin(GL_LINES);

		math::AABB boundingBox = AABB(localAABB);
		//boundingBox.TransformAsAABB(owner->transform->transformMatrix);

		//x Axis

		glVertex3f(boundingBox.MinX(), boundingBox.MinY(), boundingBox.MinZ());
		glVertex3f(boundingBox.MaxX(), boundingBox.MinY(), boundingBox.MinZ());
		glVertex3f(boundingBox.MinX(), boundingBox.MaxY(), boundingBox.MinZ());
		glVertex3f(boundingBox.MaxX(), boundingBox.MaxY(), boundingBox.MinZ());
		glVertex3f(boundingBox.MinX(), boundingBox.MaxY(), boundingBox.MaxZ());
		glVertex3f(boundingBox.MaxX(), boundingBox.MaxY(), boundingBox.MaxZ());
		glVertex3f(boundingBox.MinX(), boundingBox.MinY(), boundingBox.MaxZ());
		glVertex3f(boundingBox.MaxX(), boundingBox.MinY(), boundingBox.MaxZ());

		glVertex3f(boundingBox.MinX(), boundingBox.MinY(), boundingBox.MinZ());
		glVertex3f(boundingBox.MinX(), boundingBox.MaxY(), boundingBox.MinZ());
		glVertex3f(boundingBox.MaxX(), boundingBox.MinY(), boundingBox.MinZ());
		glVertex3f(boundingBox.MaxX(), boundingBox.MaxY(), boundingBox.MinZ());
		glVertex3f(boundingBox.MaxX(), boundingBox.MinY(), boundingBox.MaxZ());
		glVertex3f(boundingBox.MaxX(), boundingBox.MaxY(), boundingBox.MaxZ());
		glVertex3f(boundingBox.MinX(), boundingBox.MinY(), boundingBox.MaxZ());
		glVertex3f(boundingBox.MinX(), boundingBox.MaxY(), boundingBox.MaxZ());
				   					  					  
		glVertex3f(boundingBox.MinX(), boundingBox.MinY(), boundingBox.MinZ());
		glVertex3f(boundingBox.MinX(), boundingBox.MinY(), boundingBox.MaxZ());
		glVertex3f(boundingBox.MaxX(), boundingBox.MinY(), boundingBox.MinZ());
		glVertex3f(boundingBox.MaxX(), boundingBox.MinY(), boundingBox.MaxZ());
		glVertex3f(boundingBox.MaxX(), boundingBox.MaxY(), boundingBox.MinZ());
		glVertex3f(boundingBox.MaxX(), boundingBox.MaxY(), boundingBox.MaxZ());
		glVertex3f(boundingBox.MinX(), boundingBox.MaxY(), boundingBox.MinZ());
		glVertex3f(boundingBox.MinX(), boundingBox.MaxY(), boundingBox.MaxZ());

		glEnd();
		
	}
	//-- UnBind Buffers--//
	

	//--Disables States--//
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	
	App->renderer3D->wireframeMode ? glPolygonMode(GL_FRONT_AND_BACK, GL_LINE) : glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	if (drawFaceNormals || drawVertexNormals)
		DrawNormals();

	return true;
}

void ComponentMesh::OnGui()
{
	if (ImGui::CollapsingHeader("Mesh"))
	{
		ImGui::Text("Num vertices %d", numVertices);
		ImGui::Text("Num faces %d", numIndices / 3);
		ImGui::Text("Mesh path: %s", meshPath.c_str());
		ImGui::Text("Texture path: %s", texturePath.c_str());
		ImGui::Checkbox("Wireframe", &drawWireframe);
		ImGui::DragFloat("Normal draw scale", &normalScale);
		ImGui::Checkbox("Draw face normals", &drawFaceNormals);
		ImGui::Checkbox("Draw vertex normals", &drawVertexNormals);
		ImGui::Text("AABB position: %f %f %f", centerPoint.x, centerPoint.y, centerPoint.z);
			
	}
}

bool ComponentMesh::OnSave(JSONWriter& writer)
{
	writer.String("meshPath"); writer.String(meshPath.c_str());
	writer.String("texturePath"); writer.String(texturePath.c_str());

	//SAVE_JSON_STRING(meshPath.c_str);
	//SAVE_JSON_STRING(texturePath.c_str());

	return true;
}

bool ComponentMesh::OnLoad(JSONReader& reader)
{
	if (reader.HasMember("meshPath"))
	{
		std::string str = reader["meshPath"].GetString();
		App->import->LoadGeometry(str.c_str(), owner);
	}
	

	return true;
}

 
void ComponentMesh::SaveMesh() 
{
	std::ofstream saveFile;
	saveFile.open(meshPath + ".msh", std::ios::in | std::ios::app | std::ios::binary);
	if (saveFile.is_open()) {
		/*int indiceSize = indices.size() * sizeof(uint);
		int verticesSize = vertices.size() * sizeof(float) * 3;
		int normalsSize = normals.size() * sizeof(float) * 3;
		int textCoordsSize = texCoords.size() * sizeof(float) * 2;
		int faceNormalsSize = faceNormals.size() * sizeof(float) * 3;
		int faceCentersSize = faceCenters.size() * sizeof(float) * 3;

		uint sizes[6] = { vertices.size(), normals.size(), texCoords.size(), indices.size(), faceNormals.size(), faceCenters.size() };
		uint size = sizeof(sizes) + indiceSize
			+ verticesSize
			+ normalsSize
			+ textCoordsSize
			+ faceNormalsSize
			+ faceCentersSize;

		char* fileBuffer = new char[size];
		char* cursor = fileBuffer;

		uint bytes = sizeof(sizes);
		memcpy(cursor, sizes, bytes);
		cursor += bytes;

		newFile.write(fileBuffer, sizeof(fileBuffer));*/

		saveFile.write((char*)&indices[0], indices.size() * sizeof(float) * 3);
	}
	saveFile.close();

	std::ifstream openFile;
	openFile.open(meshPath + ".msh", std::ios::in | std::ios::app | std::ios::binary);
	if (openFile.is_open())
	{
		
	}

	std::string logg = "Successfully saved mesh information in " + meshPath + ".msh";
	LOG(logg.c_str());
}