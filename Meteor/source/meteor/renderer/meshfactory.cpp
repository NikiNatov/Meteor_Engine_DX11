#include "pch.h"
//#include "meshfactory.h"
//
//#include "meteor\math\noise.h"
//
//namespace meteor
//{
//    // --------------------------------------------------------------------------------------------------------------------------------------
//    Ref<Mesh> MeshFactory::CreateCube(mtrFloat size, Ref<MaterialInstance> material)
//    {
//		std::vector<Vertex> vertices;
//		vertices.reserve(24);
//
//		glm::vec3 positions[8];
//		glm::vec3 normals[6];
//		glm::vec2 texCoords[14];
//
//		positions[0] = glm::vec3(-size / 2.0f, -size / 2.0f, -size / 2.0f);
//		positions[1] = glm::vec3( size / 2.0f, -size / 2.0f, -size / 2.0f);
//		positions[2] = glm::vec3(-size / 2.0f,  size / 2.0f, -size / 2.0f);
//		positions[3] = glm::vec3( size / 2.0f,  size / 2.0f, -size / 2.0f);
//		positions[4] = glm::vec3(-size / 2.0f, -size / 2.0f,  size / 2.0f);
//		positions[5] = glm::vec3( size / 2.0f, -size / 2.0f,  size / 2.0f);
//		positions[6] = glm::vec3(-size / 2.0f,  size / 2.0f,  size / 2.0f);
//		positions[7] = glm::vec3( size / 2.0f,  size / 2.0f,  size / 2.0f);
//
//		normals[0] = glm::vec3(0.0f, 1.0f, 0.0f);
//		normals[1] = glm::vec3(0.0f, 0.0f, 1.0f);
//		normals[2] = glm::vec3(-1.0f, 0.0f, 0.0f);
//		normals[3] = glm::vec3(0.0f, -1.0f, 0.0f);
//		normals[4] = glm::vec3(1.0f, 0.0f, 0.0f);
//		normals[5] = glm::vec3(0.0f, 0.0f, -1.0f);
//
//		texCoords[0]  = glm::vec2(0.000f, 0.333f);
//		texCoords[1]  = glm::vec2(0.250f, 0.333f);
//		texCoords[2]  = glm::vec2(0.250f, 0.000f);
//		texCoords[3]  = glm::vec2(0.500f, 0.000f);
//		texCoords[4]  = glm::vec2(0.500f, 0.333f);
//		texCoords[5]  = glm::vec2(0.750f, 0.333f);
//		texCoords[6]  = glm::vec2(1.000f, 0.333f);
//		texCoords[7]  = glm::vec2(1.000f, 0.666f);
//		texCoords[8]  = glm::vec2(0.750f, 0.666f);
//		texCoords[9]  = glm::vec2(0.500f, 0.666f);
//		texCoords[10] = glm::vec2(0.500f, 1.000f);
//		texCoords[11] = glm::vec2(0.250f, 1.000f);
//		texCoords[12] = glm::vec2(0.250f, 0.666f);
//		texCoords[13] = glm::vec2(0.000f, 0.666f);
//
//		// Top face
//		vertices.push_back({ positions[4], texCoords[12], normals[2] });
//		vertices.push_back({ positions[2], texCoords[0],  normals[2] });
//		vertices.push_back({ positions[0], texCoords[13], normals[2] });
//
//		vertices.push_back({ positions[2], texCoords[2], normals[0] });
//		vertices.push_back({ positions[7], texCoords[4], normals[0] });
//		vertices.push_back({ positions[3], texCoords[3], normals[0] });
//
//		vertices.push_back({ positions[6], texCoords[1], normals[1] });
//		vertices.push_back({ positions[5], texCoords[9], normals[1] });
//		vertices.push_back({ positions[7], texCoords[4], normals[1] });
//
//		vertices.push_back({ positions[1], texCoords[8], normals[4] });
//		vertices.push_back({ positions[7], texCoords[4], normals[4] });
//		vertices.push_back({ positions[5], texCoords[9], normals[4] });
//
//		vertices.push_back({ positions[0], texCoords[7], normals[5] });
//		vertices.push_back({ positions[3], texCoords[5], normals[5] });
//		vertices.push_back({ positions[1], texCoords[8], normals[5] });
//
//		vertices.push_back({ positions[4], texCoords[12], normals[3] });
//		vertices.push_back({ positions[1], texCoords[10], normals[3] });
//		vertices.push_back({ positions[5], texCoords[9],  normals[3] });
//
//
//		vertices.push_back({ positions[6], texCoords[1], normals[0] });
//		vertices.push_back({ positions[6], texCoords[1], normals[1] });
//		vertices.push_back({ positions[4], texCoords[12], normals[2] });
//
//		vertices.push_back({ positions[3], texCoords[5], normals[3] });
//		vertices.push_back({ positions[2], texCoords[6], normals[4] });
//		vertices.push_back({ positions[0], texCoords[11], normals[5] });
//
//		std::vector<mtrU32> indices =
//		{
//			0, 1, 2,
//			3, 4, 5,
//			6, 7, 8,
//			9, 10, 11,
//			12, 13, 14,
//			15, 16, 17,
//			0, 18, 1,
//			3, 19, 4,
//			6, 20, 7,
//			9, 21, 10,
//			12, 22, 13,
//			15, 23, 16
//		};
//
//		Ref<Mesh> mesh = CreateRef<Mesh>(vertices);
//		mesh->SetName("CubeMesh");
//		mesh->AddMaterial(material);
//		mesh->SetSubmeshCount(2);
//		mesh->SetSubmeshIndices(0, indices);
//		mesh->SetSubmeshIndices(1, { 0, 1, 2, 16, 2 });
//
//		return mesh;
//    }
//
//    // --------------------------------------------------------------------------------------------------------------------------------------
//    Ref<Mesh> MeshFactory::CreatePlane(mtrFloat width, mtrFloat height, Ref<MaterialInstance> material)
//    {
//		std::vector<Vertex> vertices;
//		vertices.resize(4);
//
//		vertices[0].Position	= glm::vec3(-width / 2.0f, 0.0f, height / 2.0f);
//		vertices[0].TexCoords	= glm::vec2(0.0f, 0.0f);
//		vertices[0].Normal		= glm::vec3(0.0f, 1.0f, 0.0f);
//
//		vertices[1].Position	= glm::vec3(width / 2.0f, 0.0f, height / 2.0f);
//		vertices[1].TexCoords	= glm::vec2(1.0f, 0.0f);
//		vertices[1].Normal		= glm::vec3(0.0f, 1.0f, 0.0f);
//
//		vertices[2].Position	= glm::vec3(width / 2.0f, 0.0f, -height / 2.0f);
//		vertices[2].TexCoords	= glm::vec2(1.0f, 1.0f);
//		vertices[2].Normal		= glm::vec3(0.0f, 1.0f, 0.0f);
//
//		vertices[3].Position	= glm::vec3(-width / 2.0f, 0.0f, -height / 2.0f);
//		vertices[3].TexCoords	= glm::vec2(0.0f, 1.0f);
//		vertices[3].Normal		= glm::vec3(0.0f, 1.0f, 0.0f);
//
//		std::vector<uint32_t> indices = {
//			0, 1, 2, 2, 3, 0
//		};
//
//		Ref<Mesh> mesh = CreateRef<Mesh>(vertices);
//		mesh->SetName("PlaneMesh");
//		mesh->AddMaterial(material);
//		mesh->SetSubmeshCount(1);
//		mesh->SetSubmeshIndices(0, indices);
//
//		return mesh;
//    }
//
//    // --------------------------------------------------------------------------------------------------------------------------------------
//    Ref<Mesh> MeshFactory::CreateGrid(mtrU32 numVerticesX, mtrU32 numVerticesZ, mtrFloat cellSize, Ref<MaterialInstance> material)
//    {
//		std::vector<Vertex> vertices;
//		vertices.reserve(numVerticesX * numVerticesZ);
//
//		for (mtrU32 i = 0; i < numVerticesZ; i++)
//		{
//			for (mtrU32 j = 0; j < numVerticesX; j++)
//			{
//				Vertex vertex;
//
//				mtrFloat x = ((mtrFloat)j / ((mtrFloat)numVerticesX - 1) - 0.5f) * ((numVerticesX - 1) * cellSize);
//				mtrFloat z = ((mtrFloat)i / ((mtrFloat)numVerticesZ - 1) - 0.5f) * ((numVerticesZ - 1) * cellSize);
//				mtrFloat y = 0.0f;
//
//				vertex.Position = { x, y, z };
//				vertex.Normal = { 0.0f, 1.0f, 0.0f };
//				vertex.TexCoords = { ((mtrFloat)j / (mtrFloat)numVerticesX) , ((mtrFloat)i / (mtrFloat)numVerticesZ) };
//
//				vertices.push_back(vertex);
//			}
//		}
//
//		std::vector<mtrU32> indices;
//		mtrU32 numIndices = (numVerticesX - 1) * (numVerticesZ - 1) * 8;
//		indices.reserve(numIndices);
//
//		for (mtrU32 i = 0; i < numVerticesZ - 1; i++)
//		{
//			for (mtrU32 j = 0; j < numVerticesX - 1; j++)
//			{
//				indices.push_back(j + i * numVerticesX);
//				indices.push_back(j + 1 + i * numVerticesX);
//				indices.push_back(j + 1 + i * numVerticesX);
//				indices.push_back(j + 1 + (i + 1) * numVerticesX);
//				indices.push_back(j + 1 + (i + 1) * numVerticesX);
//				indices.push_back(j + (i + 1) * numVerticesX);
//				indices.push_back(j + (i + 1) * numVerticesX);
//				indices.push_back(j + i * numVerticesX);
//			}
//		}
//
//		Ref<Mesh> mesh = CreateRef<Mesh>(vertices);
//		mesh->SetName("GridMesh");
//		mesh->AddMaterial(material);
//		mesh->SetSubmeshCount(1);
//		mesh->SetSubmeshIndices(0, indices);
//		mesh->SetSubmeshTopology(0, PrimitiveTopology::Lines);
//
//		return mesh;
//    }
//}
