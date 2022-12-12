#include "conv.h"

namespace qcxutil
{
	QVector3D vec2qvector(const trimesh::vec3& vertex)
	{
		return QVector3D(vertex.x, vertex.y, vertex.z);
	}

	QVector<QVector3D> vecs2qvectors(const std::vector<trimesh::vec3>& vertices)
	{
		QVector<QVector3D> results;
		for (const trimesh::vec3& vertex : vertices)
			results.append(vec2qvector(vertex));
		return results;
	}

	QMatrix4x4 xform2QMatrix(const trimesh::fxform& xf)
	{
		QMatrix4x4 m;
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				m(i, j) = xf(i, j);
			}
		}
		return m;
	}

	trimesh::fxform qMatrix2Xform(const QMatrix4x4& matrix)
	{
		trimesh::fxform xf;
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				xf(i, j) = matrix(i, j);
			}
		}
		return xf;
	}

	qtuser_3d::Box3D triBox2Box3D(const trimesh::box3& box)
	{
		QVector3D bmin = QVector3D(box.min.x, box.min.y, box.min.z);
		QVector3D bmax = QVector3D(box.max.x, box.max.y, box.max.z);
		qtuser_3d::Box3D box3d(bmin, bmax);
		return box3d;
	}

	trimesh::vec3 qVector3D2Vec3(const QVector3D& vec)
	{
		return trimesh::vec3(vec.x(), vec.y(), vec.z());
	}

	cxnd::Ray qRay2CRay(const qtuser_3d::Ray& ray)
	{
		return cxnd::Ray(qVector3D2Vec3(ray.start),
			qVector3D2Vec3(ray.dir));
	}
}