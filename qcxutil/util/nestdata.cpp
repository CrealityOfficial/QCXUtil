#include "nestdata.h"

#include "qhullWrapper/hull/meshconvex.h"
#include "polygonLib/polygonLib.h"

namespace qcxutil
{
    NestData::NestData()
        : m_dirty(true)
    {
    }

    NestData::~NestData()
    {
    }

    void NestData::setDirty(bool dirty)
    {
        m_dirty = dirty;
    }

    bool NestData::dirty()
    {
        return m_dirty;
    }

    std::vector<trimesh::vec3> NestData::path(TriMeshPtr hull, const trimesh::vec3& scale, bool simple)
    {
        calculateXYConvex(hull, trimesh::fromQuaterian(rotation), scale);

        const std::vector<trimesh::vec3>& p = cPath(simple);

        std::vector<trimesh::vec3> result;
        for (const trimesh::vec3& v : p)
        {
            result.push_back(trimesh::vec3(v.x, v.y, 0.0f));
        };
        return result;
    }

    const std::vector<trimesh::vec3>& NestData::cPath(bool simple)
    {
        if (simple)
            return simples;
        return convex ? convex->vertices : simples;
    }

    void NestData::setNestRotation(const trimesh::quaternion& _rotation)
    {
        rotation = _rotation;
    }

    trimesh::quaternion NestData::nestRotation()
    {
        return rotation;
    }

    std::vector<trimesh::vec3> NestData::concave_path(TriMeshPtr globalMesh)
    {
        std::vector<trimesh::vec3> lines;
        if (globalMesh)
        {
            globalMesh->need_bbox();
            trimesh::vec3 localPosition = globalMesh->bbox.center();

            double angle = 0.0; //nestRotation;
            double sinAng = std::sin(-angle * M_PIl / 180.0f);
            double cosAng = std::cos(-angle * M_PIl / 180.0f);
            auto moveAndRotPoint = [sinAng, cosAng](trimesh::vec3 pt, trimesh::vec3 axis)
            {
                pt = pt - axis;
                trimesh::vec3 result;
                result.x = pt.x * cosAng - pt.y * sinAng;
                result.y = pt.x * sinAng + pt.y * cosAng;
                return result;
            };

            std::vector<polygonLib::PointF> concaveHull;
            for (const trimesh::point& pt : globalMesh->vertices)
            {
                trimesh::vec3 pt_rot = moveAndRotPoint(pt, localPosition);
                concaveHull.push_back(polygonLib::PointF(pt_rot.x, pt_rot.y));
            }
            concaveHull = polygonLib::PolygonPro::polygonConcaveHull(concaveHull);

            for (const polygonLib::PointF& v : concaveHull)
            {
                lines.push_back(trimesh::vec3(v.x, v.y, 0.0f));
            }
        }

        return lines;
    }

    void NestData::copyData(const NestData* nd)
    {
        if (!nd)
            return;

        convex = nd->convex;
        simples = nd->simples;
        m_dirty = nd->m_dirty;
        rotation = nd->rotation;
    }

    void NestData::calculateXYConvex(TriMeshPtr hull, const trimesh::fxform& rxf, const trimesh::vec3& scale)
    {
        if (!dirty())
            return;

        if (hull)
        {
            convex.reset(qhullWrapper::convex2DPolygon(hull.get(), rxf, nullptr));

            for (trimesh::vec3& v : convex->vertices)
                v *= scale;

            trimesh::box3 box;
            for (const trimesh::vec3& v : convex->vertices)
                box += v;

            simples.clear();
            const trimesh::vec3& dmin = box.min;
            const trimesh::vec3& dmax = box.max;
            simples.push_back(trimesh::vec3(dmin.x, dmin.y, 0.0f));
            simples.push_back(trimesh::vec3(dmax.x, dmin.y, 0.0f));
            simples.push_back(trimesh::vec3(dmax.x, dmax.y, 0.0f));
            simples.push_back(trimesh::vec3(dmin.x, dmax.y, 0.0f));
            setDirty(false);
        }
    }
}