#include "renderprimitive.h"
#include "qtuser3d/geometry/bufferhelper.h"
#include "qtuser3d/geometry/geometrycreatehelper.h"

#include "mmesh/create/createcube.h"

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
}