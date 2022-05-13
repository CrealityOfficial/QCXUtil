#ifndef QCXUTIL_INTERFACE_1604911737496_H
#define QCXUTIL_INTERFACE_1604911737496_H
#include "ccglobal/export.h"

#if USE_QCXUTIL_DLL
	#define QCXUTIL_API CC_DECLARE_IMPORT
#elif USE_QCXUTIL_STATIC
	#define QCXUTIL_API CC_DECLARE_STATIC
#else
	#if QCXUTIL_DLL
		#define QCXUTIL_API CC_DECLARE_EXPORT
	#else
		#define QCXUTIL_API CC_DECLARE_STATIC
	#endif
#endif

#endif // QCXUTIL_INTERFACE_1604911737496_H