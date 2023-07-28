#ifndef QCXUTIL_VEC2QVECTOR_1645445878366_H
#define QCXUTIL_VEC2QVECTOR_1645445878366_H
#include "qcxutil/interface.h"
#include "trimesh2/Vec.h"
#include "trimesh2/XForm.h"
#include "trimesh2/Box.h"
#include "trimesh2/quaternion.h"
#include "cxnd/struct/ray.h"

#include <QtCore/QVector>
#include <QtGui/QMatrix4x4>
#include <QtGui/QVector3D>
#include <QtGui/QQuaternion>

#include "qtuser3d/math/box3d.h"
#include "qtuser3d/math/ray.h"

namespace qcxutil
{
	QCXUTIL_API QVector3D vec2qvector(const trimesh::vec3& vertex);
	QCXUTIL_API QVector<QVector3D> vecs2qvectors(const std::vector<trimesh::vec3>& vertices);
	QCXUTIL_API QMatrix4x4 xform2QMatrix(const trimesh::fxform& xf);
	QCXUTIL_API qtuser_3d::Box3D triBox2Box3D(const trimesh::box3& box);

	QCXUTIL_API trimesh::fxform qMatrix2Xform(const QMatrix4x4& matrix);
	QCXUTIL_API trimesh::vec3 qVector3D2Vec3(const QVector3D& vec);
	QCXUTIL_API trimesh::box3 qBox32box3(const qtuser_3d::Box3D& box);
	QCXUTIL_API cxnd::Ray qRay2CRay(const qtuser_3d::Ray& ray);
	QCXUTIL_API trimesh::quaternion qQuaternion2tQuaternion(const QQuaternion& q);
	QCXUTIL_API QQuaternion tqua2qqua(trimesh::quaternion q);
}

#endif // QCXUTIL_VEC2QVECTOR_1645445878366_H