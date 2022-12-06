#ifndef QCXUTIL_VEC2QVECTOR_1645445878366_H
#define QCXUTIL_VEC2QVECTOR_1645445878366_H
#include "qcxutil/interface.h"
#include "trimesh2/Vec.h"
#include "trimesh2/XForm.h"
#include "trimesh2/Box.h"

#include <QtCore/QVector>
#include <QtGui/QMatrix4x4>
#include <QtGui/QVector3D>

#include "qtuser3d/math/box3d.h"

namespace qcxutil
{
	QCXUTIL_API QVector3D vec2qvector(const trimesh::vec3& vertex);
	QCXUTIL_API QVector<QVector3D> vecs2qvectors(const std::vector<trimesh::vec3>& vertices);
	QCXUTIL_API QMatrix4x4 xform2QMatrix(const trimesh::fxform& xf);

	QCXUTIL_API trimesh::fxform qMatrix2Xform(const QMatrix4x4& matrix);
	QCXUTIL_API qtuser_3d::Box3D triBox2Box3D(const trimesh::box3& box);
}

#endif // QCXUTIL_VEC2QVECTOR_1645445878366_H