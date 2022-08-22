#ifndef QCXUTIL_QUAZIPFILE_1660289457034_H
#define QCXUTIL_QUAZIPFILE_1660289457034_H
#include "qcxutil/interface.h"
#include <QtCore/QObject>
#include <QtCore/QIODevice>
#include <memory>
#include <functional>

typedef std::function<bool(QIODevice& device)> QuazipSubFunc;

class QuaZip;
namespace qcxutil
{
	class QCXUTIL_API QuazipFile : public QObject
	{
		Q_OBJECT
	public:
		QuazipFile(QObject* parent = nullptr);
		virtual ~QuazipFile();

		void open(const QString& fileName);
		bool openSubFile(const QString& subFileName, QuazipSubFunc func);
	protected:
		std::unique_ptr<QuaZip> m_zip;
	};
}

#endif // QCXUTIL_QUAZIPFILE_1660289457034_H