#include "renderprimitive.h"
#include "qtuser3d/geometry/bufferhelper.h"
#include "qtuser3d/geometry/geometrycreatehelper.h"

#include "mmesh/create/createcube.h"
#include "mmesh/trimesh/algrithm3d.h"

namespace qcxutil
{
	static unsigned static_box_triangles_indices[36] = {
		2, 0, 3,
		2, 1, 0,
		0, 1, 5,
		0, 5, 4,
		1, 2, 6,
		1, 6, 5,
		2, 3, 7,
		2, 7, 6,
		3, 0, 4,
		3, 4, 7,
		4, 5, 6,
		4, 6, 7
	};

	static unsigned static_box_part_indices[48] = {
		0, 1,
		0, 2,
		0, 3,

		4, 5,
		4, 6,
		4, 7,

		8, 9,
		8, 10,
		8, 11,

		12, 13,
		12, 14,
		12, 15,

		16, 17,
		16, 18,
		16, 19,

		20, 21,
		20, 22,
		20, 23,

		24, 25,
		24, 26,
		24, 27,

		28, 29,
		28, 30,
		28, 31

	};

	Qt3DRender::QGeometry* createLinesGeometry(trimesh::vec3* lines, int num)
	{
		if (num == 0 || !lines)
			return nullptr;

		Qt3DRender::QAttribute* positionAttribute = qtuser_3d::BufferHelper::CreateVertexAttribute(
			(const char*)lines, num, 3, Qt3DRender::QAttribute::defaultPositionAttributeName());

		return qtuser_3d::GeometryCreateHelper::create(nullptr, positionAttribute);
	}

	Qt3DRender::QGeometry* createIndicesGeometry(trimesh::vec3* positions, int num, int* indices, int tnum)
	{
		if (num == 0 || !positions || tnum == 0 || !indices)
			return nullptr;

		Qt3DRender::QAttribute* positionAttribute = qtuser_3d::BufferHelper::CreateVertexAttribute(
			(const char*)positions, num, 3, Qt3DRender::QAttribute::defaultPositionAttributeName());
		Qt3DRender::QAttribute* indexAttribute = qtuser_3d::BufferHelper::CreateIndexAttribute((const char*)indices, tnum);

		return qtuser_3d::GeometryCreateHelper::create(nullptr, positionAttribute, indexAttribute);
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
			auto genDatasFromCorner = [](std::vector<trimesh::vec3>& positions, int pos,
				const trimesh::vec3& boxsz, const trimesh::vec3& dir)
			{
				positions[pos + 1 + 0] = positions[pos] + trimesh::vec3(boxsz.x * dir.x, 0.0f, 0.0f);
				positions[pos + 1 + 1] = positions[pos] + trimesh::vec3(0.0f, boxsz.y * dir.y, 0.0f);
				positions[pos + 1 + 2] = positions[pos] + trimesh::vec3(0.0f, 0.0f, boxsz.z * dir.z);
			};

			std::vector<trimesh::vec3> positions;
			positions.resize(32);

			trimesh::vec3 bmin = box.min;
			trimesh::vec3 bmax = box.max;

			trimesh::vec3 boxsz = (bmax - bmin) * (parameter.ratio / 2.0f);

			positions[0] = bmin;
			genDatasFromCorner(positions, 0, boxsz, trimesh::vec3(1.0f, 1.0f, 1.0f));

			positions[4] = trimesh::vec3(bmax.x, bmin.y, bmin.z);
			genDatasFromCorner(positions, 4, boxsz, trimesh::vec3(-1.0f, 1.0f, 1.0f));

			positions[8] = trimesh::vec3(bmax.x, bmax.y, bmin.z);
			genDatasFromCorner(positions, 8, boxsz, trimesh::vec3(-1.0f, -1.0f, 1.0f));

			positions[12] = trimesh::vec3(bmin.x, bmax.y, bmin.z);
			genDatasFromCorner(positions, 12, boxsz, trimesh::vec3(1.0f, -1.0f, 1.0f));

			positions[16] = trimesh::vec3(bmin.x, bmin.y, bmax.z);
			genDatasFromCorner(positions, 16, boxsz, trimesh::vec3(1.0f, 1.0f, -1.0f));

			positions[20] = trimesh::vec3(bmax.x, bmin.y, bmax.z);
			genDatasFromCorner(positions, 20, boxsz, trimesh::vec3(-1.0f, 1.0f, -1.0f));

			positions[24] = bmax;
			genDatasFromCorner(positions, 24, boxsz, trimesh::vec3(-1.0f, -1.0f, -1.0f));

			positions[28] = trimesh::vec3(bmin.x, bmax.y, bmax.z);
			genDatasFromCorner(positions, 28, boxsz, trimesh::vec3(1.0f, -1.0f, -1.0f));

			return createIndicesGeometry(positions.data(), 32, (int*)static_box_part_indices, 48);;
		}

		std::vector<trimesh::vec3> lines;
		mmesh::boxLines(box, lines);

		return createLinesGeometry(lines);
	}

	Qt3DRender::QGeometry* createCubeTriangles(const trimesh::box3& box)
	{
		std::vector<trimesh::vec3> positions;
		positions.resize(8);

		trimesh::vec3 bmin = box.min;
		trimesh::vec3 bmax = box.max;

		positions[0] = bmin;
		positions[1] = trimesh::vec3(bmax.x, bmin.y, bmin.z);
		positions[2] = trimesh::vec3(bmax.x, bmax.y, bmin.z);
		positions[3] = trimesh::vec3(bmin.x, bmax.y, bmin.z);
		positions[4] = trimesh::vec3(bmin.x, bmin.y, bmax.z);
		positions[5] = trimesh::vec3(bmax.x, bmin.y, bmax.z);
		positions[6] = bmax;
		positions[7] = trimesh::vec3(bmin.x, bmax.y, bmax.z);

		return createIndicesGeometry(positions.data(), 24, (int*)static_box_triangles_indices, 36);
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