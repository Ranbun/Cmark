#ifndef CAMERAMARK_CLOG_H
#define CAMERAMARK_CLOG_H

#include <QDebug>

namespace CM
{
    class CLog
    {
    public:
        static void Init();

        template<typename T>
        void PrintMes(const T & value)
        {
            assert(this);

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

    private:
        static std::once_flag m_InitFlag;

    };


    inline CLog g_Log;
    #define CLogInstance CM::g_Log

} // CM

#endif //CAMERAMARK_CLOG_H
