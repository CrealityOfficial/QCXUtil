#ifndef QCXUTIL_Q3DRENDER_1645445878366_H
#define QCXUTIL_Q3DRENDER_1645445878366_H
#include "qcxutil/interface.h"
#include "trimesh2/Vec.h"
#include "trimesh2/XForm.h"
#include "trimesh2/Box.h"
#include "trimesh2/TriMesh.h"

#include "qtuser3d/geometry/attribute.h"
#include <Qt3DRender/QGeometry>
#include <QtCore/QByteArray>

namespace qcxutil
{
	QCXUTIL_API Qt3DRender::QGeometry* trimeshes2Geometry(const std::vector<trimesh::TriMesh*>& meshes, Qt3DCore::QNode* parent = nullptr);
	QCXUTIL_API Qt3DRender::QGeometry* trimesh2Geometry(trimesh::TriMesh* mesh, Qt3DCore::QNode* parent = nullptr);

	QCXUTIL_API QByteArray createFlagArray(int faceNum);
	QCXUTIL_API QByteArray createPositionArray(trimesh::TriMesh* mesh);

	QCXUTIL_API void trimeshes2AttributeShade(const std::vector<trimesh::TriMesh*>& meshes, qtuser_3d::AttributeShade& position, qtuser_3d::AttributeShade& normal);
	QCXUTIL_API void trimesh2AttributeShade(trimesh::TriMesh* mesh, qtuser_3d::AttributeShade& position, qtuser_3d::AttributeShade& normal);

}

#endif // QCXUTIL_Q3DRENDER_1645445878366_H