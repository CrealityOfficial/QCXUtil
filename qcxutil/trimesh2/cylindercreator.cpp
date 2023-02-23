#include "cylindercreator.h"

namespace creative_kernel
{
	CylinderCreator::CylinderCreator(int n)
		:m_n(n)
	{

	}

	CylinderCreator::~CylinderCreator()
	{

	}

	int CylinderCreator::vertexNum()
	{
		return 2 * m_n;
	}

	int CylinderCreator::faceNum()
	{
		return 2 * m_n;
	}

	void CylinderCreator::fill(const trimesh::vec3& start, const trimesh::vec3& end,
		trimesh::vec3* positions, trimesh::vec3* normals)
	{

	}
}