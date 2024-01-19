#ifndef CAMERAMARK_CLOG_H
#define CAMERAMARK_CLOG_H

#include <QDebug>

namespace CM
{
    class CLog
    {
    public:
        template<typename T>
        static void Print(const T & value)
        {
            auto typeName = QMetaType::typeName(qMetaTypeId<T>());
            if(typeName)
            {
                qDebug() << value;
            }
        }
        template<typename T>
        static void Info(const T & value)
        {
            auto typeName = QMetaType::typeName(qMetaTypeId<T>());
            if(typeName)
            {
                qInfo() << value;
            }
        }

        template<typename T>
        static void Warning(const T & value)
        {
            auto typeName = QMetaType::typeName(qMetaTypeId<T>());
            if(typeName)
            {
                qWarning() << value;
            }
        }

        template<typename T>
        static void Fatal(const T & value)
        {
            auto typeName = QMetaType::typeName(qMetaTypeId<T>());
            if(typeName)
            {
                qFatal() << value;
            }
        }

    };

} // CM

#endif //CAMERAMARK_CLOG_H
