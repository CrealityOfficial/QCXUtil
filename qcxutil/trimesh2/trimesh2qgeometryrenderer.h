#ifndef Q_UTIL_TRIMESH_2_GEOMETRY_RENDERER_H
#define Q_UTIL_TRIMESH_2_GEOMETRY_RENDERER_H
#include "qcxutil/interface.h"
#include "trimesh2/TriMesh.h"
#include "qtuser3d/geometry/attribute.h"
#include <Qt3DRender/QGeometry>

namespace qcxutil
{
	QCXUTIL_API void generateGeometryDataFromMesh(trimesh::TriMesh* mesh, qtuser_3d::GeometryData& data);
	QCXUTIL_API Qt3DRender::QGeometry* createGeometryFromMesh(trimesh::TriMesh* mesh, Qt3DCore::QNode* parent = nullptr);

	QCXUTIL_API Qt3DRender::QGeometry* trimeshes2Geometry(std::vector<trimesh::TriMesh*>& meshes, int vflag = -1, Qt3DCore::QNode* parent = nullptr);
	QCXUTIL_API Qt3DRender::QGeometry* trimesh2Geometry(trimesh::TriMesh* mesh, int vflag = -1, Qt3DCore::QNode* parent = nullptr);
}

#endif // Q_UTIL_TRIMESH_2_GEOMETRY_RENDERER_H