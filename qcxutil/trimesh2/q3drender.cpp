#include "q3drender.h"

#include <Qt3DRender/QBuffer>
#include "qtuser3d/geometry/geometrycreatehelper.h"

#include "qtusercore/module/glcompatibility.h"

namespace qcxutil
{
	Qt3DRender::QGeometry* trimesh2Geometry(trimesh::TriMesh* mesh, Qt3DCore::QNode* parent)
	{
		if (!mesh)
			return nullptr;

		std::vector<trimesh::TriMesh*> meshes;
		meshes.push_back(mesh);
		return trimeshes2Geometry(meshes, parent);
	}

	Qt3DRender::QGeometry* trimeshes2Geometry(const std::vector<trimesh::TriMesh*>& meshes, Qt3DCore::QNode* parent)
	{
		int count = 0;
		for (trimesh::TriMesh* mesh : meshes)
			count += (int)mesh->faces.size() * 3;

		if (count <= 0)
			return nullptr;

		QByteArray position;
#if QT_USE_GLES
		int vertexStride = 4;
		position.resize(count * 4 * sizeof(float));
		trimesh::vec4* vertexData = (trimesh::vec4*)position.data();
#else
		int vertexStride = 3;
		position.resize(count * 3 * sizeof(float));
		trimesh::vec3* vertexData = (trimesh::vec3*)position.data();
#endif

		QByteArray normal;
		normal.resize(count * 3 * sizeof(float));
		trimesh::vec3* normalData = (trimesh::vec3*)normal.data();

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
					const trimesh::vec3& v = mesh->vertices.at(f[j]);
#if QT_USE_GLES
					vertexData[index] = trimesh::vec4(v.x, v.y, v.z, (float)index);
#else
					vertexData[index] = v;
#endif
					normalData[index] = n;
					++index;
				}
			}
		}

		Qt3DRender::QBuffer* positionBuffer = new Qt3DRender::QBuffer(Qt3DRender::QBuffer::VertexBuffer);
		Qt3DRender::QBuffer* normalBuffer = new Qt3DRender::QBuffer(Qt3DRender::QBuffer::VertexBuffer);
		positionBuffer->setData(position);
		normalBuffer->setData(normal);

		Qt3DRender::QAttribute* positionAttribute = new Qt3DRender::QAttribute(positionBuffer, Qt3DRender::QAttribute::defaultPositionAttributeName(), Qt3DRender::QAttribute::Float, vertexStride, count);
		Qt3DRender::QAttribute* normalAttribute = new Qt3DRender::QAttribute(normalBuffer, Qt3DRender::QAttribute::defaultNormalAttributeName(), Qt3DRender::QAttribute::Float, 3, count);
		return qtuser_3d::GeometryCreateHelper::create(parent, positionAttribute, normalAttribute);
	}
}