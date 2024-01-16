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
            // auto typeName = QMetaType::typeName(QMetaTypeId<T>());
            auto typeName = "N";
            if(typeName)
            {
                qDebug() << value << endl;
            }
        }

    };

} // CM

#endif //CAMERAMARK_CLOG_H
