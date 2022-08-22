#include "quazipfile.h"
#include "quazip/quazip.h"
#include "quazip/quazipfile.h"

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

	bool QuazipFile::openSubFile(const QString& subFileName, QuazipSubFunc func)
	{
		if (!m_zip)
			return false;

		QuaZipFile file;

		m_zip->setCurrentFile(subFileName);
		file.setZip(m_zip.get());
		if (file.open(QIODevice::ReadOnly) && func)
		{
			func(file);
			file.close();
			return true;
		}

		file.close();
		return false;
	}
}