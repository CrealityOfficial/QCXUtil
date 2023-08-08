#ifndef QCXUTIL_RENDERPRIMITIVE_1681808843406_H
#define QCXUTIL_RENDERPRIMITIVE_1681808843406_H
#include "qcxutil/interface.h"
#include "trimesh2/Box.h"
#include <vector>
#include <Qt3DRender/QGeometry>

namespace qcxutil
{
	QCXUTIL_API Qt3DRender::QGeometry* createLinesGeometry(trimesh::vec3* lines, int num);
	QCXUTIL_API Qt3DRender::QGeometry* createIndicesGeometry(trimesh::vec3* positions, int num, int* indices, int tnum);

	QCXUTIL_API Qt3DRender::QGeometry* createLinesGeometry(const std::vector<trimesh::vec3>& lines);

	struct CubeParameter
	{
		bool parts = false;
		float ratio = 1.0f;
	};

	QCXUTIL_API Qt3DRender::QGeometry* createCubeLines(const trimesh::box3& box, const CubeParameter& parameter = CubeParameter());
	QCXUTIL_API Qt3DRender::QGeometry* createCubeTriangles(const trimesh::box3& box);

	struct GridParameter
	{
		float delta = 20.0f;
		int xNum = 10;
		int yNum = 10;
		trimesh::vec3 offset = trimesh::vec3(0.0f, 0.0f, 0.0f);
	};

	QCXUTIL_API Qt3DRender::QGeometry* createGridLines(const GridParameter& parameter = GridParameter());
	QCXUTIL_API Qt3DRender::QGeometry* createMidGridLines(const trimesh::box3& box, float gap = 10.0f, float offset = 5.0f);

	QCXUTIL_API Qt3DRender::QGeometry* createIdentityTriangle();
	QCXUTIL_API Qt3DRender::QGeometry* createSimpleTriangle(const trimesh::vec3& v1, const trimesh::vec3& v2, const trimesh::vec3& v3);

	QCXUTIL_API Qt3DRender::QGeometry* createSimpleQuad();
	QCXUTIL_API Qt3DRender::QGeometry* createTriangles(const std::vector<trimesh::vec3>& tris);

	QCXUTIL_API Qt3DRender::QGeometry* createPoints(const std::vector<trimesh::vec3>& points);
}

#endif // QCXUTIL_RENDERPRIMITIVE_1681808843406_H