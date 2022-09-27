#include "cxcontent.h"

namespace qcxutil
{
	CXContent readContentFromStream(QXmlStreamReader& reader)
	{
        CXContent content;
        bool have_init_pos = false;
        while (!reader.atEnd())
        {
            QXmlStreamReader::TokenType type = reader.readNext();
            switch (type) {
                case QXmlStreamReader::NoToken:
                    break;
                case QXmlStreamReader::Invalid:
                    break;
                case QXmlStreamReader::StartDocument:
                    break;
                case QXmlStreamReader::EndDocument:
                    break;
                //case QXmlStreamReader::StartElement:
                //{
                //    if ("version" == reader.name().toString())
                //    {
                //        parseQString(m_pXmlReader->attributes(), m_contentxml->version);
                //    }
                //    else if ("machine" == reader.name().toString())
                //    {
                //        parseQString(m_pXmlReader->attributes(), m_contentxml->machine);
                //    }
                //    else if ("meshPathName" == reader.name().toString())
                //    {
                //        parseQVecQString(m_pXmlReader->attributes(), m_contentxml->meshPathName);
                //    }
                //    else if ("initposition" == reader.name().toString())
                //    {
                //        have_init_pos = true;
                //        parseVector3D(m_pXmlReader->attributes(), mMatrixTemp.InitPosition);
                //    }
                //    else if ("position" == reader.name().toString())
                //    {
                //        parseVector3D(m_pXmlReader->attributes(), mMatrixTemp.Position);
                //        if (!have_init_pos)
                //        {
                //            mMatrixTemp.InitPosition = mMatrixTemp.Position;
                //        }
                //    }
                //    else if ("scale" == reader.name().toString())
                //    {
                //        parseVector3D(m_pXmlReader->attributes(), mMatrixTemp.Scale);
                //    }
                //    else if ("rotate" == reader.name().toString())
                //    {
                //        parseQQuaternion(m_pXmlReader->attributes(), mMatrixTemp.Rotate);
                //    }
                //}
                //break;
                //case QXmlStreamReader::EndElement://元素(节点)结束
                //{
                //    QString str;
                //    str += "</";
                //    str += m_pXmlReader->name().toString();
                //    str += ">";
                //
                //    if ("support" == m_pXmlReader->name().toString())
                //    {
                //        m_contentxml->meshMatrix.push_back(mMatrixTemp);
                //        have_init_pos = false;
                //        mMatrixTemp.reset();
                //    }
                //    break;
                //}
                case QXmlStreamReader::Comment:
                {
                    QString str;
                    str += "<!--";
                    str += reader.text().toString();
                    str += "-->";
                    break;
                }
                case QXmlStreamReader::EntityReference:
                    break;
                case QXmlStreamReader::ProcessingInstruction:
                    break;
            }
        }

        return content;
	}
}