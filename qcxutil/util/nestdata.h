#ifndef QCXUTIL_NESTDATA_1685692131729_H
#define QCXUTIL_NESTDATA_1685692131729_H
#include "qcxutil/interface.h"
#include "qcxutil/trimesh2/conv.h"
#include "trimesh2/TriMesh.h"
#include <memory>

typedef std::shared_ptr<trimesh::TriMesh> TriMeshPtr;

namespace qcxutil
{
    class QCXUTIL_API NestData
    {
    public:
        NestData();
        virtual ~NestData();

        void copyData(const NestData* nd);

        void setDirty(bool dirty);
        bool dirty();

        std::vector<trimesh::vec3> path(TriMeshPtr hull, const trimesh::vec3& scale = trimesh::vec3(1.0f, 1.0f, 1.0f), bool simple = false);
        std::vector<trimesh::vec3> qPath(TriMeshPtr hull, const trimesh::quaternion& rotation, const trimesh::vec3& scale = trimesh::vec3(1.0f, 1.0f, 1.0f), bool simple = false);

        const std::vector<trimesh::vec3>& cPath(bool simple = false);
        std::vector<trimesh::vec3> concave_path(TriMeshPtr globalMesh);

        void setNestRotation(const trimesh::quaternion& rotation);
        trimesh::quaternion nestRotation();
    protected:
        void calculateXYConvex(TriMeshPtr hull, const trimesh::fxform& rxf = trimesh::fxform::identity(),
            const trimesh::vec3& scale = trimesh::vec3(1.0f, 1.0f, 1.0f));
    protected:
        TriMeshPtr convex;
        std::vector<trimesh::vec3> simples;
        trimesh::quaternion rotation;
  
        bool m_dirty;
    };

    typedef std::shared_ptr<NestData> NestDataPtr;
}

#endif // QCXUTIL_NESTDATA_1685692131729_H