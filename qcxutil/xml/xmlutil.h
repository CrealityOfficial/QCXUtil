#ifndef QCXUTIL_II_XML_UTIL_H
#define QCXUTIL_II_XML_UTIL_H

#include "qcxutil/interface.h"

#include <QtCore/QXmlStreamWriter>
#include <QtCore/QXmlStreamReader>
#include <QtCore/QXmlStreamAttributes>
#include <QtCore/QVector>
#include <QtGui/QVector3D>
#include <QtGui/QQuaternion>
#include <QtGui/QMatrix4x4>

#include "trimesh2/TriMesh.h"

namespace qcxutil
{
	QCXUTIL_API QString qMatrix2QString(const QMatrix4x4& matrix, QString split = " ");

	QCXUTIL_API int qstring2QMatrix(const QString& ms, QMatrix4x4& matrix, QString split = " ");

	QCXUTIL_API QString getStartDocumentTag(const QXmlStreamReader& reader, int type = 0);

	QCXUTIL_API int parseVector3D(const QXmlStreamAttributes& attributes, QVector3D& v);

	QCXUTIL_API int parseQQuaternion(const QXmlStreamAttributes& attributes, QQuaternion& v);

	QCXUTIL_API int parseQString(const QXmlStreamAttributes& attributes, QString& v);

	QCXUTIL_API int parseVecQString(const QXmlStreamAttributes& attributes, std::vector<QString>& v);

	QCXUTIL_API int parseQVecQString(const QXmlStreamAttributes& attributes, QVector<QString>& v);

	QCXUTIL_API int parseQMatrix4x4(const QXmlStreamAttributes& attributes, QMatrix4x4& m);

	QCXUTIL_API int writeQQuaternion(QXmlStreamWriter& writer, const QString& qualifiedName, const QQuaternion& v);

	QCXUTIL_API int writeVector3D(QXmlStreamWriter& writer, const QString& qualifiedName, const QVector3D& v);

	QCXUTIL_API int writeTrimeshPoint(QXmlStreamWriter& writer, const QString& qualifiedName, const trimesh::point& v);

	QCXUTIL_API int writeTrimeshFace(QXmlStreamWriter& writer, const QString& qualifiedName, const trimesh::TriMesh::Face& v);
}
#endif // QCXUTIL_II_XML_UTIL_H