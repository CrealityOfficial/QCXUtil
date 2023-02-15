#ifndef CREATIVE_KERNEL_PARAMETERUTIL_1676022156634_H
#define CREATIVE_KERNEL_PARAMETERUTIL_1676022156634_H
#include <QtCore/QString>
#include <QtCore/QStringList>

namespace qcxutil
{
    // object has name func
	template<class T>
	QStringList object2List(const QList<T*>& objects)
	{
        QStringList list;
        for (T* object : objects)
        {
            if (!list.contains(object->name()))
                list.append(object->name());
            else
            {
                qWarning() << QString("Dumplicate object name [%1]").arg(object->name());
            }
        }
        return list;
	}

    // object has name func
    template<class T>
    T* findObject(const QString& name, const QList<T*>& objects)
    {
        T* result = nullptr;
        for (T* object : objects)
        {
            if (object->name() == name)
            {
                result = object;
                break;
            }
        }
        return result;
    }
}

#endif // CREATIVE_KERNEL_PARAMETERUTIL_1676022156634_H