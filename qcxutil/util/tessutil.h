#ifndef _TESS_UTIL_H_
#define _TESS_UTIL_H_
#include "qcxutil/interface.h"
#include "qcxutil/trimesh2/conv.h"

namespace qcxutil
{
    QCXUTIL_API void tessPolygon(const std::vector<std::vector<trimesh::vec3>>& polygons, std::vector<trimesh::vec3>& vertexs);
}
#endif // _TESS_UTIL_H_