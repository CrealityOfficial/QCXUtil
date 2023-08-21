__cc_find(Qrlib)
__cc_find(Trimesh2)
__cc_find(OpenSSL)
__cc_find(Quazip)
__cc_find(zlib)
__cc_find(NestPlacer)
__cc_find(Clipper3r)
__cc_find(Clipper)
__cc_find(libtess2)

__assert_target(qrlib)
__assert_target(trimesh2)
__assert_target(quazip)
__assert_target(ssl)
__assert_target(nestplacer)
__assert_target(clipper3r)
__assert_target(libtess2)
__assert_target(topomesh)

set(SRCS qcxutil/interface.h
		 qcxutil/qrcode/qrcode.h
		 qcxutil/qrcode/qrcode.cpp
		 qcxutil/trimesh2/conv.h
		 qcxutil/trimesh2/conv.cpp
		 qcxutil/trimesh2/q3drender.h
		 qcxutil/trimesh2/q3drender.cpp
		 qcxutil/trimesh2/cylindercreator.h
		 qcxutil/trimesh2/cylindercreator.cpp
		 qcxutil/xml/xmlutil.h
		 qcxutil/xml/xmlutil.cpp
		 qcxutil/crypt/cryptfiledevice.h
		 qcxutil/crypt/cryptfiledevice.cpp
		 qcxutil/quazip/quazipfile.h
		 qcxutil/quazip/quazipfile.cpp
		 qcxutil/xml/cxcontent.h
		 qcxutil/xml/cxcontent.cpp
		 qcxutil/trimesh2/trimesh2qgeometryrenderer.h
		 qcxutil/trimesh2/trimesh2qgeometryrenderer.cpp
		 qcxutil/util/traitsutil.h
		 qcxutil/util/nest2djob.h
		 qcxutil/util/nest2djob.cpp
		 qcxutil/util/nestdata.h
		 qcxutil/util/nestdata.cpp
		 qcxutil/util/tessutil.h
		 qcxutil/util/tessutil.cpp
		 qcxutil/trimesh2/renderprimitive.h
		 qcxutil/trimesh2/renderprimitive.cpp
		 qcxutil/letter/lettermodel.h
		 qcxutil/letter/lettermodel.cpp
		 )
		 
set(LIBS qtuser_core qtuser_3d Qt5::Core Qt5::Gui Qt5::3DRender Qt5::Qml qrlib trimesh2 ssl crypto
		 quazip zlib mmesh nestplacer clipper3r clipper libnest2d qhullWrapper libtess2 topomesh
		)
set(ILIBS cxnd)

if(CC_BC_LINUX)
	list(APPEND LIBS pthread)
endif()				
if( WIN32 AND NOT CYGWIN )
	set(LIBS ${LIBS} crypt32 ws2_32)
endif()

__add_common_library(qcxutil)

if( WIN32 AND NOT CYGWIN )
	target_link_libraries(qcxutil PRIVATE crypt32 ws2_32)
endif()