__cc_find(Qrlib)
__cc_find(Trimesh2)
__assert_target(qrlib)
__assert_target(trimesh2)

set(SRCS qcxutil/interface.h
		 qcxutil/qrcode/qrcode.h
		 qcxutil/qrcode/qrcode.cpp
		 qcxutil/trimesh2/conv.h
		 qcxutil/trimesh2/conv.cpp
		 qcxutil/trimesh2/q3drender.h
		 qcxutil/trimesh2/q3drender.cpp
		 qcxutil/xml/xmlutil.h
		 qcxutil/xml/xmlutil.cpp
		 )
		 
set(LIBS qtuser_core qtuser_3d Qt5::Core Qt5::Gui Qt5::3DRender qrlib trimesh2)
__add_common_library(qcxutil)