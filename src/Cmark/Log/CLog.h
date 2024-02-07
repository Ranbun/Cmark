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
            if(auto typeName = QMetaType::typeName(qMetaTypeId<T>()))
            {
                qDebug() << value;
            }
        }
        template<typename T>
        static void Info(const T & value)
        {
            if(auto typeName = QMetaType::typeName(qMetaTypeId<T>()))
            {
                qInfo() << value;
            }
        }

        template<typename T>
        static void Warning(const T & value)
        {
            if(auto typeName = QMetaType::typeName(qMetaTypeId<T>()))
            {
                qWarning() << value;
            }
        }

        template<typename T>
        static void Fatal(const T & value)
        {
            Q_ASSERT(false);
        }

        template<>
        static void Fatal(const std::string& value)
        {
            qFatal("%s",value.c_str());
        }
    };

} // CM

#endif //CAMERAMARK_CLOG_H
