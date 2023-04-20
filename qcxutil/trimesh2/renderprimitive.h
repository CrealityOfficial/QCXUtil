#ifndef QCXUTIL_RENDERPRIMITIVE_1681808843406_H
#define QCXUTIL_RENDERPRIMITIVE_1681808843406_H
#include "qcxutil/interface.h"
#include "trimesh2/Box.h"
#include <vector>
#include <Qt3DRender/QGeometry>

namespace qcxutil
{
	QCXUTIL_API Qt3DRender::QGeometry* createLinesGeometry(trimesh::vec3* lines, int num);
	QCXUTIL_API Qt3DRender::QGeometry* createLinesGeometry(const std::vector<trimesh::vec3>& lines);

	struct CubeParameter
	{
		bool parts = false;
		float ratio = 1.0f;
	};

	QCXUTIL_API Qt3DRender::QGeometry* createCubeLines(const trimesh::box3& box, const CubeParameter& parameter = CubeParameter());

	struct GridParameter
	{
		float delta = 20.0f;
		int xNum = 10;
		int yNum = 10;
		trimesh::vec3 offset = trimesh::vec3(0.0f, 0.0f, 0.0f);
	};

	QCXUTIL_API Qt3DRender::QGeometry* createGridLines(const GridParameter& parameter = GridParameter());
}

#endif // QCXUTIL_RENDERPRIMITIVE_1681808843406_H