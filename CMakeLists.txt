__assert_target(qrlib)

set(SRCS qcxutil/interface.h
		 qcxutil/qrcode/qrcode.h
		 qcxutil/qrcode/qrcode.cpp
		 )
		 
set(LIBS Qt5::Core Qt5::Gui qrlib)
set(DEFS QCXUTIL_DLL)

__add_real_target(qcxutil dll SOURCE ${SRCS}
							  LIB ${LIBS}
							  DEF ${DEFS}
							  INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
							  SOURCE_FOLDER
							  )