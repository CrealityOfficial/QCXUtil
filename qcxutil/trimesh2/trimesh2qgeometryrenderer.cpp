#include "trimesh2qgeometryrenderer.h"

#include "qtuser3d/geometry/geometrycreatehelper.h"
#include "qtuser3d/math/angles.h"
#include <Qt3DRender/QBuffer>

namespace qcxutil
{
	
	void generateGeometryDataFromMesh(trimesh::TriMesh* mesh, qtuser_3d::GeometryData& data)
	{
		if (!mesh || (mesh->faces.size() == 0) || (mesh->vertices.size() == 0))
			return;

		int fcount = (int)mesh->faces.size();
		int count = fcount * 3;

		data.fcount = fcount;
		data.vcount = count;

		QByteArray& positionByteArray = data.position;
		QByteArray& normalByteArray = data.normal;
		QByteArray& texcoordArray = data.texcoord;
		QByteArray& colorArray = data.color;

		positionByteArray.resize(count * 3 * sizeof(float));
		normalByteArray.resize(count * 3 * sizeof(float));

		bool hasTex = true;
		if(hasTex)
			texcoordArray.resize(count * 2 * sizeof(float));

		bool hasColor = mesh->colors.size() > 0;
		if (hasColor)
			colorArray.resize(count * 3 * sizeof(float));

		if (count == (int)mesh->vertices.size())
		{
			memcpy(positionByteArray.data(), &mesh->vertices[0], positionByteArray.size());
			if (mesh->normals.size() != count)
			{
				mesh->need_normals();
				memcpy(normalByteArray.data(), &mesh->normals[0], normalByteArray.size());
			}
		}
		else
		{
			trimesh::vec3* vertexData = (trimesh::vec3*)positionByteArray.data();
			trimesh::vec3* normalData = (trimesh::vec3*)normalByteArray.data();
			trimesh::vec2* texcoordData = (trimesh::vec2*)texcoordArray.data();
			trimesh::vec3* colorData = (trimesh::vec3*)colorArray.data();

			for (int i = 0; i < fcount; ++i)
			{
				if (hasColor)
				{
					trimesh::Color c = mesh->colors.at(i);
					for (int j = 0; j < 3; ++j)
					{
						colorData[i * 3 + j] = c;
					}
				}

				trimesh::TriMesh::Face& f = mesh->faces.at(i);
				for (int j = 0; j < 3; ++j)
				{
					vertexData[i * 3 + j] = mesh->vertices.at(f[j]);
				}

				trimesh::vec3 v01 = mesh->vertices.at(f[1]) - mesh->vertices.at(f[0]);
				trimesh::vec3 v02 = mesh->vertices.at(f[2]) - mesh->vertices.at(f[0]);
				trimesh::vec3 n = v01 TRICROSS v02;
				trimesh::normalize(n);;

				for (int j = 0; j < 3; ++j)
				{
					normalData[i * 3 + j] = n;
				}

				if (texcoordArray.size() && hasTex)
				{
					if (i >= mesh->faceUVs.size()
						|| i >= mesh->textureIDs.size())
					{
						//确保传入shader中的数据正常
						texcoordData[3 * i] = trimesh::vec2(-1.0, -1.0);
						texcoordData[3 * i + 1] = trimesh::vec2(-1.0, -1.0);
						texcoordData[3 * i + 2] = trimesh::vec2(-1.0, -1.0);
					}
					else
					{
						trimesh::TriMesh::Face& faceUV = mesh->faceUVs.at(i);

						trimesh::vec2 uv0(-1.0, -1.0);
						trimesh::vec2 uv1(-1.0, -1.0);
						trimesh::vec2 uv2(-1.0, -1.0);

						int textureID = mesh->textureIDs[i];
						if (textureID > -1 && !mesh->materials.empty())
						{
							const trimesh::Material& material = mesh->materials[textureID];
							const trimesh::vec2& map_startUV = material.map_startUVs[trimesh::Material::MapType::DIFFUSE];
							const trimesh::vec2& map_endUV = material.map_endUVs[trimesh::Material::MapType::DIFFUSE];
							int UVsizeMax = mesh->UVs.size();
							if (faceUV.x > -1.0 && (faceUV.x < UVsizeMax))
							{
								uv0 = mesh->UVs.at(faceUV.x);
								uv0 = map_startUV + uv0 * (map_endUV - map_startUV);
							}
							if (faceUV.y > -1.0 && (faceUV.y < UVsizeMax))
							{
								uv1 = mesh->UVs.at(faceUV.y);
								uv1 = map_startUV + uv1 * (map_endUV - map_startUV);
							}
							if (faceUV.z > -1.0 && (faceUV.z < UVsizeMax))
							{
								uv2 = mesh->UVs.at(faceUV.z);
								uv2 = map_startUV + uv2 * (map_endUV - map_startUV);
							}
						}

						texcoordData[3 * i] = trimesh::vec2(uv0.x, uv0.y);
						texcoordData[3 * i + 1] = trimesh::vec2(uv1.x, uv1.y);
						texcoordData[3 * i + 2] = trimesh::vec2(uv2.x, uv2.y);

#if 1
						texcoordData[3 * i] = trimesh::vec2(-1.0, -1.0);
						texcoordData[3 * i + 1] = trimesh::vec2(-1.0, -1.0);
						texcoordData[3 * i + 2] = trimesh::vec2(-1.0, -1.0);
#endif
					}
				}
			}
		}
	}
	

	Qt3DRender::QGeometry* createGeometryFromMesh(trimesh::TriMesh* mesh, Qt3DCore::QNode* parent)
	{
		qtuser_3d::GeometryData data;
		generateGeometryDataFromMesh(mesh, data);

		return qtuser_3d::GeometryCreateHelper::create(data, parent);
	}

	Qt3DRender::QGeometry* trimesh2Geometry(trimesh::TriMesh* mesh, int vflag, Qt3DCore::QNode* parent)
	{
		if (!mesh)
			return nullptr;

		std::vector<trimesh::TriMesh*> meshes;
		meshes.push_back(mesh);
		return trimeshes2Geometry(meshes, vflag, parent);
	}

	Qt3DRender::QGeometry* trimeshes2Geometry(std::vector<trimesh::TriMesh*>& meshes, int vflag, Qt3DCore::QNode* parent)
	{
		int count = 0;
		for (trimesh::TriMesh* mesh : meshes)
			count += (int)mesh->faces.size() * 3;

		if (count <= 0)
			return nullptr;

		QByteArray position;
		QByteArray normal;
		position.resize(count * 3 * sizeof(float));
		normal.resize(count * 3 * sizeof(float));
		QByteArray flag;

		if (vflag >= 0)
		{
			flag.resize(count * sizeof(float));
			flag.fill(0);
		}

		trimesh::vec3* vertexData = (trimesh::vec3*)position.data();
		trimesh::vec3* normalData = (trimesh::vec3*)normal.data();
		float* flagData = (float*)flag.data();
		int index = 0;
		for (trimesh::TriMesh* mesh : meshes)
		{
			for (trimesh::TriMesh::Face& f : mesh->faces)
			{
				const trimesh::vec3& v0 = mesh->vertices.at(f[0]);
				const trimesh::vec3& v1 = mesh->vertices.at(f[1]);
				const trimesh::vec3& v2 = mesh->vertices.at(f[2]);

				trimesh::vec3 v01 = v1 - v0;
				trimesh::vec3 v02 = v2 - v0;
				trimesh::vec3 n = v01 TRICROSS v02;
				trimesh::normalize(n);

				for (int j = 0; j < 3; ++j)
				{
					vertexData[index] = mesh->vertices.at(f[j]);
					normalData[index] = n;
					if (vflag >= 0)
					{
						flagData[index] = (float)vflag;
					}
					++index;
				}
			}
		}

		Qt3DRender::QBuffer* positionBuffer = new Qt3DRender::QBuffer(Qt3DRender::QBuffer::VertexBuffer);
		Qt3DRender::QBuffer* normalBuffer = new Qt3DRender::QBuffer(Qt3DRender::QBuffer::VertexBuffer);
		Qt3DRender::QBuffer* flagBuffer = nullptr;
		if (vflag >= 0)
			flagBuffer = new Qt3DRender::QBuffer(Qt3DRender::QBuffer::VertexBuffer);
		positionBuffer->setData(position);
		normalBuffer->setData(normal);
		if (flagBuffer)
			flagBuffer->setData(flag);
		Qt3DRender::QAttribute* positionAttribute = new Qt3DRender::QAttribute(positionBuffer, Qt3DRender::QAttribute::defaultPositionAttributeName(), Qt3DRender::QAttribute::Float, 3, count);
		Qt3DRender::QAttribute* normalAttribute = new Qt3DRender::QAttribute(normalBuffer, Qt3DRender::QAttribute::defaultNormalAttributeName(), Qt3DRender::QAttribute::Float, 3, count);
		Qt3DRender::QAttribute* flagAttribute = nullptr;
		if (vflag >= 0)
			flagAttribute = new Qt3DRender::QAttribute(flagBuffer, "vertexFlag", Qt3DRender::QAttribute::Float, 1, count);

		return qtuser_3d::GeometryCreateHelper::create(parent, positionAttribute, normalAttribute, flagAttribute);
	}

	void generateIndexGeometryDataFromMesh(trimesh::TriMesh* mesh, qtuser_3d::GeometryData& data)
	{
		if (!mesh || (mesh->faces.size() == 0) || (mesh->vertices.size() == 0))
			return;

		int fcount = (int)mesh->faces.size();
		int vcount = mesh->vertices.size();

		data.fcount = fcount;
		data.vcount = vcount;
		data.indiceCount = fcount * 3;

		QByteArray& positionByteArray = data.position;
		QByteArray& indiceArray = data.indices;

		positionByteArray.resize(data.vcount * 3 * sizeof(float));

		indiceArray.resize(fcount * 3 * sizeof(uint));

		uint* indiceData = (uint*)indiceArray.data();
		for (int i = 0; i < fcount; ++i)
		{
			trimesh::TriMesh::Face& f = mesh->faces.at(i);
			for (int j = 0; j < 3; ++j)
			{
				indiceData[i * 3 + j] = f[j];
			}

		}

		float* vertexData = (float*)positionByteArray.data();
		for (int i = 0; i < mesh->vertices.size(); i++)
		{
			for (int j = 0; j < 3; j++)
			{
				vertexData[i * 3 + j] = mesh->vertices.at(i).at(j);
			}

		}

	}
}



