#ifndef _SURF_RECON_H_
#define _SURF_RECON_H_
#include "qcxutil/interface.h"
#include "trimesh2/TriMesh.h"
#include "ccglobal/tracer.h"

namespace qcxutil
{
    QCXUTIL_API trimesh::TriMesh* SurfaceRebuild(const char* path, ccglobal::Tracer* tracer);

}
#endif // _TESS_UTIL_H_