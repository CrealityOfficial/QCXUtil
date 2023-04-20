#include "renderprimitive.h"
#include "qtuser3d/geometry/bufferhelper.h"
#include "qtuser3d/geometry/geometrycreatehelper.h"

#include "mmesh/create/createcube.h"
#include "mmesh/trimesh/algrithm3d.h"

namespace qcxutil
{
	Qt3DRender::QGeometry* createLinesGeometry(trimesh::vec3* lines, int num)
	{
		if (num == 0 || !lines)
			return nullptr;

		Qt3DRender::QAttribute* positionAttribute = qtuser_3d::BufferHelper::CreateVertexAttribute(
			(const char*)lines, num, 3, Qt3DRender::QAttribute::defaultPositionAttributeName());

		return qtuser_3d::GeometryCreateHelper::create(nullptr, positionAttribute);
	}

	Qt3DRender::QGeometry* createLinesGeometry(const std::vector<trimesh::vec3>& lines)
	{
		int size = (int)lines.size();
		return createLinesGeometry((size > 0 ? (trimesh::vec3*)&lines.at(0) : nullptr), size);
	}

	Qt3DRender::QGeometry* createCubeLines(const trimesh::box3& box, const CubeParameter& parameter)
	{
		if (parameter.parts)
		{
			return nullptr;
		}

		std::vector<trimesh::vec3> lines;
		mmesh::boxLines(box, lines);

		return createLinesGeometry(lines);
	}

	Qt3DRender::QGeometry* createGridLines(const GridParameter& parameter)
	{
		std::vector<trimesh::vec3> lines;
		int n = parameter.xNum + parameter.yNum + 2;
		if (n < 2)
			return nullptr;

		int nn = 2 * n;
		lines.reserve(nn);

		float xmax = parameter.delta * (float)parameter.xNum;
		float ymax = parameter.delta * (float)parameter.yNum;
		for (int i = 0; i <= parameter.xNum; ++i)
		{
			float x = parameter.delta * (float)i;
			lines.push_back(trimesh::vec3(x, 0.0f, 0.0f));
			lines.push_back(trimesh::vec3(x, ymax, 0.0f));
		}

		for (int i = 0; i <= parameter.yNum; ++i)
		{
			float y = parameter.delta * (float)i;
			lines.push_back(trimesh::vec3(0.0f, y, 0.0f));
			lines.push_back(trimesh::vec3(xmax, y, 0.0f));
		}

		mmesh::offsetPoints(lines, parameter.offset);
		return createLinesGeometry(lines);
	}
}