#include "nestdata.h"

#include "qhullWrapper/hull/meshconvex.h"
#include "polygonLib/polygonLib.h"
#include "mmesh/util/mnode.h"
#include "cxutil/slicer/slicehelper.h"

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
        calculateXYConvex(hull, mmesh::fromQuaterian(rotation), scale);

        const std::vector<trimesh::vec3>& p = cPath(simple);

        std::vector<trimesh::vec3> result;
        for (const trimesh::vec3& v : p)
        {
            result.push_back(trimesh::vec3(v.x, v.y, 0.0f));
        };
        return result;
    }

    std::vector<trimesh::vec3> NestData::qPath(TriMeshPtr hull, const trimesh::quaternion& _rotation, const trimesh::vec3& scale, bool simple)
    {
        std::vector<trimesh::vec3> p = calculateGlobalXYConvex(hull, mmesh::fromQuaterian(_rotation), scale);

        std::vector<trimesh::vec3> result;
        for (const trimesh::vec3& v : p)
        {
            result.push_back(trimesh::vec3(v.x, v.y, 0.0f));
        };
        return result;
    }

    std::vector<trimesh::vec3> NestData::debug_path()
    {
        const std::vector<trimesh::vec3>& p = cPath(false);

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
        m_dirty = true;
    }

    trimesh::quaternion NestData::nestRotation()
    {
        return rotation;
    }

    bool find_connet(std::vector<std::pair<int, int>>& harf_edge, std::vector<int>& poly, int cur_idx)
    {
        int connet_idx = -1;
        for (int i = 0; i < harf_edge.size(); i++)
        {
            if (cur_idx == harf_edge[i].first)
            {
                connet_idx = harf_edge[i].second;
                harf_edge[i] = std::make_pair(-1, -1);
                break;
            }
            if (cur_idx == harf_edge[i].second)
            {
                connet_idx = harf_edge[i].first;
                harf_edge[i] = std::make_pair(-1, -1);
                break;
            }
        }
        if (connet_idx > -1 || connet_idx == *poly.begin())
        {
            poly.push_back(connet_idx);
            find_connet(harf_edge, poly, connet_idx);
            return true;

        }
        else
            return false;
    }

    std::vector<trimesh::vec3> NestData::concave_path(TriMeshPtr mesh, const trimesh::vec3 scalse)
    {
        auto addPoint = [](Clipper3r::Path& item, int max_dis, bool openPoly = false)
        {
            int ptSize = item.size();
            if (ptSize == 0) return;
            Clipper3r::Path itemDensed;
            itemDensed.push_back(item[0]);
            int add = openPoly ? 0 : 1;
            for (int i = 1; i < ptSize + add; i++)
            {
                int curIdx = i < ptSize ? i : i - ptSize;
                Clipper3r::IntPoint ptAdd;
                ptAdd.X = item[curIdx].X - item[i - 1].X;
                ptAdd.Y = item[curIdx].Y - item[i - 1].Y;
                int len = sqrt(ptAdd.X * ptAdd.X + ptAdd.Y * ptAdd.Y);
                if (len > max_dis)
                {
                    int addNum = len / max_dis;
                    for (int j = 1; j < addNum; j++)
                    {
                        float theta = j * max_dis / (float)len;

                        Clipper3r::IntPoint pt_theta = Clipper3r::IntPoint(theta * ptAdd.X, theta * ptAdd.Y);
                        Clipper3r::IntPoint pt_new;
                        pt_new.X = item[i - 1].X + pt_theta.X;
                        pt_new.Y = item[i - 1].Y + pt_theta.Y;
                        itemDensed.push_back(pt_new);
                    }
                }
                itemDensed.push_back(item[curIdx]);
            }
            item.swap(itemDensed);
        };

        double SCALE = 1000.0;
        std::vector<trimesh::vec3> lines;
        if (mesh)
        {
#if 1
            cxutil::SliceHelper helper;
            helper.prepare(mesh.get());

            Clipper3r::Path polys_point;
            int face_size = helper.faces.size();
            std::vector<bool> face_normal(face_size, false);
            for (int i = 0; i < face_size; i++)
            {
                const cxutil::MeshFace& face = helper.faces[i];
                trimesh::vec3& v0 = mesh.get()->vertices.at(face.vertex_index[0]);
                trimesh::vec3& v1 = mesh.get()->vertices.at(face.vertex_index[1]);
                trimesh::vec3& v2 = mesh.get()->vertices.at(face.vertex_index[2]);

                trimesh::vec3 v01 = v1 - v0;
                trimesh::vec3 v02 = v2 - v0;
                trimesh::vec3 n = v01 TRICROSS v02;
                trimesh::normalize(n);
                face_normal[i] = n.z < -0.01;
            }
            int faceNum = 0;
            std::vector<std::pair<int, int>> harf_edges;
            for (int i = 0; i < face_size; i++)
            {
                if (!face_normal[i]) continue;
                faceNum++;
                const cxutil::MeshFace& face = helper.faces[i];      
                for (int j = 0; j < 3; j++)
                {
                    if (face.connected_face_index[j] == -1 || !face_normal[face.connected_face_index[j]])
                    {
                        harf_edges.push_back(std::make_pair(face.vertex_index[j], face.vertex_index[(j + 1) % 3]));
                    }
                }
            }
#if 0
            std::vector< std::vector<int>> polys_record;
            while (!harf_edges.empty())
            {
                std::vector<int> poly_idx_record;
                poly_idx_record.reserve(harf_edges.size());
                poly_idx_record.push_back(harf_edges[0].first);
                poly_idx_record.push_back(harf_edges[0].second);
                harf_edges[0] = std::make_pair(-1, -1);
                find_connet(harf_edges, poly_idx_record, poly_idx_record.back());
                polys_record.push_back(poly_idx_record);
                std::vector<std::pair<int, int>> harf_edges_end;
                for (const std::pair<int, int>& harf_edge : harf_edges)
                {
                    if (harf_edge != std::make_pair(-1, -1))
                        harf_edges_end.push_back(harf_edge);
                }
                harf_edges.swap(harf_edges_end);
            }

            Clipper3r::Paths paths;
            for (std::vector<int> poly_idx_record : polys_record)
            {
                Clipper3r::Path poly;
                for (int idx : poly_idx_record)
                {
                    trimesh::vec3 pt_rot = rotation * mesh->vertices[idx];
                    poly.push_back(Clipper3r::IntPoint(pt_rot.x * SCALE, pt_rot.y * SCALE));
                }
                if (!Clipper3r::Orientation(poly))
                    Clipper3r::ReversePath(poly);
                paths.push_back(poly);
            }

            Clipper3r::Clipper clipper;
            clipper.AddPaths(paths, Clipper3r::ptSubject, true);
            clipper.Execute(Clipper3r::ctUnion, paths, Clipper3r::pftNonZero, Clipper3r::pftNonZero);
     
            for (Clipper3r::Path& path: paths)
            {
                addPoint(path, SCALE);
                polys_point.insert(polys_point.begin(), path.begin(), path.end());
            }
#endif   
            for (const std::pair<int, int>& harf_edge : harf_edges)
            {
                Clipper3r::Path line;
                trimesh::vec3 p0 = rotation * mesh->vertices[harf_edge.first];
                trimesh::vec3 p1 = rotation * mesh->vertices[harf_edge.second];
                line.push_back(Clipper3r::IntPoint(p0.x* SCALE, p0.y* SCALE));
                line.push_back(Clipper3r::IntPoint(p1.x* SCALE, p1.y* SCALE));
                addPoint(line, SCALE,true);
                polys_point.insert(polys_point.begin(), line.begin(), line.end());
            }

            polys_point = polygonLib::PolygonPro::polygonConcaveHull(polys_point);

            for (const Clipper3r::IntPoint& v : polys_point)
            {
                lines.push_back(trimesh::vec3(v.X / SCALE * scalse.x, v.Y / SCALE * scalse.y, 0.0f));
            }
#else
            std::vector<polygonLib::PointF> concaveHull;
            for (const trimesh::point& pt : mesh->vertices)
            {
                trimesh::vec3 pt_rot = rotation * pt;
                concaveHull.push_back(polygonLib::PointF(pt_rot.x, pt_rot.y));
            }
            concaveHull = polygonLib::PolygonPro::polygonConcaveHull(concaveHull);

            for (const polygonLib::PointF& v : concaveHull)
            {
                lines.push_back(trimesh::vec3(v.x * scalse.x, v.y * scalse.y, 0.0f));
            }
#endif
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

    std::vector<trimesh::vec3> NestData::calculateGlobalXYConvex(TriMeshPtr hull, const trimesh::fxform& rxf, const trimesh::vec3& scale)
    {
        std::vector<trimesh::vec3> convex2D;
        if (hull)
        {
            TriMeshPtr mesh(qhullWrapper::convex2DPolygon(hull.get(), rxf, nullptr));

            for (trimesh::vec3& v : mesh->vertices)
                v *= scale;

            convex2D = mesh->vertices;
        }

        return convex2D;
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