#ifndef CREATIVE_KERNEL_CYLINDERCREATOR_1677143666271_H
#define CREATIVE_KERNEL_CYLINDERCREATOR_1677143666271_H
#include "qcxutil/interface.h"
#include "trimesh2/Vec.h"
#include <vector>

namespace creative_kernel
{
	class QCXUTIL_API CylinderCreator
	{
	public:
		CylinderCreator(int n);
		~CylinderCreator();

		int vertexNum();
		int faceNum();
		void fill(const trimesh::vec3& start, const trimesh::vec3& end, 
			trimesh::vec3* positions, trimesh::vec3* normals);
	protected:
		std::vector<trimesh::vec3> m_positions;
		std::vector<trimesh::vec3> m_normals;

		int m_n;
	};
}

#endif // CREATIVE_KERNEL_CYLINDERCREATOR_1677143666271_H