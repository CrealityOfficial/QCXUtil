#include "xmlutil.h"
#include <QtCore/QFile>

namespace qcxutil
{
	QString qMatrix2QString(const QMatrix4x4& matrix, QString split)
	{
		QString ms;
		const float* f = matrix.constData();
		for (int i = 0; i < 16; i++)
		{
			ms.append(QString::number(f[i]));
			ms.append(split);
		}
		return ms;
	}

	int qstring2QMatrix(const QString& ms, QMatrix4x4& matrix, QString split)
	{
		float* f = matrix.data();
		QStringList vecstr = ms.split(split);
		if (vecstr.back().isEmpty())
		{
			vecstr.pop_back();
		}
		if (vecstr.size() != 16)
		{
			return -1;
		}
		for (int i = 0; i < 16; i++)
		{
			QString& s = vecstr[i];
			f[i] = s.toFloat();
		}

		return 0;
	}

	QString getStartDocumentTag(const QXmlStreamReader& reader, int type)
	{
		QString str;
		switch (type)
		{
		case 1:
			str += reader.documentVersion();
			str += "    ";
			str += reader.documentEncoding();
			str += "    ";
			str += reader.isStandaloneDocument() ? "yes" : "no";

			break;

		default:
			break;
		}

		return str;
	}

	int parseVector3D(const QXmlStreamAttributes& attributes, QVector3D& v)
	{
		if (attributes.size() == 3)
		{
			v.setX(attributes.at(0).value().toFloat());
			v.setY(attributes.at(1).value().toFloat());
			v.setZ(attributes.at(2).value().toFloat());

			return 0;
		}
		return -1;
	}

	int parseQQuaternion(const QXmlStreamAttributes& attributes, QQuaternion& v)
	{
		if (attributes.size() == 4)
		{
			v.setX(attributes.at(0).value().toFloat());
			v.setY(attributes.at(1).value().toFloat());
			v.setZ(attributes.at(2).value().toFloat());
			v.setScalar(attributes.at(3).value().toFloat());

			return 0;
		}
		return -1;
	}

	int parseQString(const QXmlStreamAttributes& attributes, QString& v)
	{
		if (attributes.size())
		{
			v = attributes.at(0).value().toString();
			return 0;
		}
		return -1;
	}

	int parseVecQString(const QXmlStreamAttributes& attributes, std::vector<QString>& v)
	{
		if (!attributes.isEmpty()) {
			foreach(QXmlStreamAttribute attribute, attributes) {
				v.push_back(attribute.value().toString());
			}
		}
		return v.size();
	}

	int parseQVecQString(const QXmlStreamAttributes& attributes, QVector<QString>& v)
	{
		if (!attributes.isEmpty()) {
			foreach(QXmlStreamAttribute attribute, attributes) {
				v.push_back(attribute.value().toString());
			}
		}
		return v.size();
	}

	int parseQMatrix4x4(const QXmlStreamAttributes& attributes, QMatrix4x4& m)
	{
		if (!attributes.isEmpty()) {
			return qstring2QMatrix(attributes[0].value().toString(), m);
		}
		return -1;

	}


	int writeVector3D(QXmlStreamWriter& writer, const QString& qualifiedName, const QVector3D& v)
	{
		writer.writeStartElement(qualifiedName);
		QString X = QString::number(v.x(), 'f', 7);
		QString Y = QString::number(v.y(), 'f', 7);
		QString Z = QString::number(v.z(), 'f', 7);
		writer.writeAttribute("x", X);
		writer.writeAttribute("y", Y);
		writer.writeAttribute("z", Z);
		writer.writeEndElement();

		return 0;
	}

	int writeQQuaternion(QXmlStreamWriter& writer, const QString& qualifiedName, const QQuaternion& v)
	{
		writer.writeStartElement(qualifiedName);
		QString X = QString::number(v.x(), 'f', 7);
		QString Y = QString::number(v.y(), 'f', 7);
		QString Z = QString::number(v.z(), 'f', 7);
		QString scalar = QString::number(v.scalar(), 'f', 7);

		writer.writeAttribute("X", X);
		writer.writeAttribute("Y", Y);
		writer.writeAttribute("Z", Z);
		writer.writeAttribute("scalar", scalar);
		writer.writeEndElement();

		return 0;
	}

	int writeTrimeshPoint(QXmlStreamWriter& writer, const QString& qualifiedName, const trimesh::point& v)
	{
		writer.writeStartElement(qualifiedName);
		QString X = QString::number(v[0], 'f', 7);
		QString Y = QString::number(v[1], 'f', 7);
		QString Z = QString::number(v[2], 'f', 7);
		writer.writeAttribute("x", X);
		writer.writeAttribute("y", Y);
		writer.writeAttribute("z", Z);
		writer.writeEndElement();

		return 0;
	}

	int writeTrimeshFace(QXmlStreamWriter& writer, const QString& qualifiedName, const trimesh::TriMesh::Face& v)
	{
		writer.writeStartElement(qualifiedName);
		QString X = QString::number(v[0]);
		QString Y = QString::number(v[1]);
		QString Z = QString::number(v[2]);
		writer.writeAttribute("v1", X);
		writer.writeAttribute("v2", Y);
		writer.writeAttribute("v3", Z);
		writer.writeEndElement();

		return 0;
	}

	bool parseXml(XmlParseFunc func, const QString& fileName)
	{
		QFile file(fileName);
		if (!file.open(QFile::ReadOnly | QFile::Text))
		{
			qWarning() << QString("parseXml openFile error.").arg(fileName);
			return false;
		}

		QXmlStreamReader xml(&file);
		while (!xml.atEnd() && !xml.hasError())
		{
			QXmlStreamReader::TokenType token = xml.readNext();
			if (token == QXmlStreamReader::StartDocument)
			{
				continue;
			}

			if (token == QXmlStreamReader::StartElement)
			{
				if (func)
					func(xml);

				xml.readNext();
			}
		}

		bool success = true;
		if (xml.hasError())
		{
			qWarning() << QString("parseXml parse [%1] error [%2].").arg(fileName).arg(xml.errorString());
			success = false;
		}

		xml.clear();
		file.close();
		return success;
	}
}
