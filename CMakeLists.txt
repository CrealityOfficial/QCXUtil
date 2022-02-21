__assert_target(qrlib)
__assert_target(trimesh2)

set(SRCS qcxutil/interface.h
		 qcxutil/qrcode/qrcode.h
		 qcxutil/qrcode/qrcode.cpp
		 qcxutil/trimesh2/vec2qvector.h
		 qcxutil/trimesh2/vec2qvector.cpp
		 )
		 
set(LIBS Qt5::Core Qt5::Gui qrlib trimesh2)
set(DEFS QCXUTIL_DLL)

__add_real_target(qcxutil dll SOURCE ${SRCS}
							  LIB ${LIBS}
							  DEF ${DEFS}
							  INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
							  SOURCE_FOLDER
							  )