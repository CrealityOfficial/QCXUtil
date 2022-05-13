__assert_target(qrlib)
__assert_target(trimesh2)

set(SRCS qcxutil/interface.h
		 qcxutil/qrcode/qrcode.h
		 qcxutil/qrcode/qrcode.cpp
		 qcxutil/trimesh2/conv.h
		 qcxutil/trimesh2/conv.cpp
		 )
		 
set(LIBS Qt5::Core Qt5::Gui qrlib trimesh2)
__add_common_library(qcxutil)