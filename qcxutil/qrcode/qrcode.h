#ifndef QCXUTIL_QRCODE_1645168025771_H
#define QCXUTIL_QRCODE_1645168025771_H
#include "qcxutil/interface.h"
#include <QtGui/QPixmap>

namespace qcxutil
{
	QCXUTIL_API QPixmap encodeUrl2QRCode(const QString& url, int width, int height);
}

#endif // QCXUTIL_QRCODE_1645168025771_H