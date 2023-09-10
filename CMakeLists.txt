__cc_find(Trimesh2)
__cc_find(OpenSSL)
__cc_find(Quazip)
__cc_find(zlib)
__cc_find(libtess2)

__assert_target(trimesh2)
__assert_target(quazip)
__assert_target(ssl)

__files_group(${CMAKE_CURRENT_SOURCE_DIR}/qcxutil/ SRCS)
		
set(LIBS qtuser_core trimesh2 ssl crypto
		 quazip zlib mmesh qhullWrapper libtess2
		)
set(ILIBS cxnd)

if(CC_BC_LINUX)
	list(APPEND LIBS pthread)
endif()				
if( WIN32 AND NOT CYGWIN )
	set(LIBS ${LIBS} crypt32 ws2_32)
endif()

__add_common_library(qcxutil)