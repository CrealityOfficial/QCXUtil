#ifndef QCXUTIL_CXCONTENT_1661159672560_H
#define QCXUTIL_CXCONTENT_1661159672560_H
#include "qcxutil/interface.h"
#include <vector>
#include <QtCore/QXmlStreamReader>
#include "trimesh2/Vec.h"
#include "trimesh2/quaternion.h"

namespace qcxutil
{
	struct TriMeshRef
	{
		std::string subFileName;
	};

	struct ContentModel
	{
		trimesh::vec3 position;
		trimesh::vec3 scale;
		trimesh::quaternion rotate;

		int meshIndex = -1;
	};

	struct CXContent
	{
		std::string version;
		std::string machine;

		std::vector<TriMeshRef> meshes;
		std::vector<ContentModel> models;
	};

	QCXUTIL_API CXContent readContentFromStream(QXmlStreamReader& reader);
}

#endif // QCXUTIL_CXCONTENT_1661159672560_H