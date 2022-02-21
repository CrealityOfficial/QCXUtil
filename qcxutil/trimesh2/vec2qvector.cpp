#include "vec2qvector.h"

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
}