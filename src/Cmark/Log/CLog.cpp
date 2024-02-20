#include "CLog.h"
namespace CM
{
    std::once_flag CLog::m_InitFlag;

    void CLog::Init()
    {
        std::call_once(m_InitFlag, []()
        {
            qSetMessagePattern("%{time yyyy-MM-dd hh:mm:ss}--[%{type}]--%{function}:%{message}");
        });

    }
}
