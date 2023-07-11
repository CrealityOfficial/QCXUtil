#include "surfaceRecon.h"
#include "gcodeparase/gcodeparase.h"

namespace qcxutil
{
    

    trimesh::TriMesh* SurfaceRebuild(const char* path, ccglobal::Tracer* tracer)
    {
        cxgcode::SGCodeLoadTracer* loadTracer = nullptr;
        cxgcode::SGCode* sgcode = gcode2Sgcode(path, loadTracer,  tracer);
        trimesh::TriMesh* mesh = new trimesh::TriMesh();
        std::cout << "ssss" << std::endl;
            return mesh;
    }
}