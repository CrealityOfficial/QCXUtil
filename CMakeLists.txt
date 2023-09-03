__cc_find(Qrlib)
__cc_find(Trimesh2)
__cc_find(OpenSSL)
__cc_find(Quazip)
__cc_find(zlib)
__cc_find(NestPlacer)
__cc_find(Clipper3r)
__cc_find(Clipper)
__cc_find(libtess2)
__cc_find(Boost)

__assert_target(qrlib)
__assert_target(trimesh2)
__assert_target(quazip)
__assert_target(ssl)
__assert_target(nestplacer)
__assert_target(clipper3r)
__assert_target(clipper)
__assert_target(libtess2)
__assert_target(topomesh)
__assert_target(boost_nowide)

__files_group(${CMAKE_CURRENT_SOURCE_DIR}/qcxutil/ SRCS)
		
set(LIBS qtuser_core Qt5::Core Qt5::Gui Qt5::Qml qrlib trimesh2 ssl crypto
		 quazip zlib mmesh nestplacer clipper3r clipper libnest2d qhullWrapper libtess2 topomesh cxutil boost_nowide
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