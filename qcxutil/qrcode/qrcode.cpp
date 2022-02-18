#include "qrcode.h"
#include "qrlib/urlqrcode.h"

#include <QtGui/QPainter>

namespace qcxutil
{
	QPixmap encodeUrl2QRCode(const QString& url, int width, int height)
	{
		qrlib::UrlQRCode code(url.toStdString());

		qint32 qrcode_width = code.width();
		qint32 temp_width = width;
		qint32 temp_height = height;
		double scale_x = (double)temp_width / (double)qrcode_width;
		double scale_y = (double)temp_height / (double)qrcode_width;
		QImage mainimg = QImage(temp_width, temp_height, QImage::Format_ARGB32);
		QPainter painter(&mainimg);
		QColor background(Qt::white);
		painter.setBrush(background);
		painter.setPen(Qt::NoPen);
		painter.drawRect(0, 0, temp_width, temp_height);
		QColor foreground(Qt::black);
		painter.setBrush(foreground);
		for (qint32 y = 0; y < qrcode_width; y++)
		{
			for (qint32 x = 0; x < qrcode_width; x++)
			{
				unsigned char b = code.pixel(x, y);
				if (b & 0x01)
				{
					QRectF r(x * scale_x, y * scale_y, scale_x, scale_y);
					painter.drawRects(&r, 1);
				}
			}
		}
		QPixmap mainmap = QPixmap::fromImage(mainimg);
		return mainmap;
	}
}