#include "quazipfile.h"
#include "quazip/quazip.h"

namespace qcxutil
{
	QuazipFile::QuazipFile(QObject* parent)
		: QObject(parent)
	{

	}

	QuazipFile::~QuazipFile()
	{
		if (m_zip)
			m_zip->close();
		m_zip.reset();
	}

	void QuazipFile::open(const QString& fileName)
	{
		m_zip.reset(new QuaZip());
		m_zip->setZipName(fileName);
		if (!m_zip->open(QuaZip::mdUnzip))
			m_zip.reset();
	}
}