#include "CLog.h"

#include <chrono>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>

namespace CM
{
    std::once_flag CLog::m_InitFlag;

    namespace
    {
        std::mutex g_LogMutex;
        bool g_ConsoleEnabled = true;
        bool g_FileEnabled = false;
        std::filesystem::path g_LogPath;
        std::ofstream g_File;

        bool IsDebugBuild()
        {
#if defined(_DEBUG)
            return true;
#elif !defined(NDEBUG)
            return true;
#else
            return false;
#endif
        }

        std::string NowString()
        {
            using namespace std::chrono;
            const auto now = system_clock::now();
            const auto tt = system_clock::to_time_t(now);
            const auto ms = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;

            std::tm tm{};
#if defined(_WIN32)
            localtime_s(&tm, &tt);
#else
            localtime_r(&tt, &tm);
#endif

            std::ostringstream oss;
            oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S") << '.'
                << std::setw(3) << std::setfill('0') << ms.count();
            return oss.str();
        }

        void EnsureFileOpenedLocked()
        {
            if (!g_FileEnabled)
                return;

            if (g_LogPath.empty())
            {
                g_LogPath = std::filesystem::current_path() / "logs" / "Cmark.log";
            }

            std::error_code ec;
            std::filesystem::create_directories(g_LogPath.parent_path(), ec);

            if (!g_File.is_open())
            {
                g_File.open(g_LogPath, std::ios::out | std::ios::app);
            }
        }
    } // namespace

    void CLog::Init()
    {
        std::call_once(m_InitFlag, []()
        {
            const bool debug = IsDebugBuild();
            g_ConsoleEnabled = debug;  // Debug：默认输出到终端
            g_FileEnabled = !debug;    // Release：默认输出到文件
            // 文件默认路径会在首次写入时延迟创建/打开
        });
    }

    void CLog::EnsureInit()
    {
        Init();
    }

    void CLog::SetLogFilePath(std::filesystem::path path)
    {
        EnsureInit();
        std::scoped_lock lk(g_LogMutex);
        g_LogPath = std::move(path);
        if (g_File.is_open())
            g_File.close();
        EnsureFileOpenedLocked();
    }

    std::filesystem::path CLog::GetLogFilePath()
    {
        EnsureInit();
        std::scoped_lock lk(g_LogMutex);
        if (g_LogPath.empty())
            g_LogPath = std::filesystem::current_path() / "logs" / "Cmark.log";
        return g_LogPath;
    }

    void CLog::EnableConsole(bool enabled)
    {
        EnsureInit();
        std::scoped_lock lk(g_LogMutex);
        g_ConsoleEnabled = enabled;
    }

    void CLog::EnableFile(bool enabled)
    {
        EnsureInit();
        std::scoped_lock lk(g_LogMutex);
        g_FileEnabled = enabled;
        if (!g_FileEnabled && g_File.is_open())
        {
            g_File.close();
        }
        EnsureFileOpenedLocked();
    }

    void CLog::Write(Level level, std::string_view message)
    {
        EnsureInit();

        std::scoped_lock lk(g_LogMutex);
        const std::string line = NowString() + "--[" + std::string(LevelToString(level)) + "]--" + std::string(message);

        if (g_ConsoleEnabled)
        {
            std::ostream &os = (level == Level::Info) ? std::clog : std::cerr;
            os << line << std::endl;
        }

        if (g_FileEnabled)
        {
            EnsureFileOpenedLocked();
            if (g_File.is_open())
            {
                g_File << line << std::endl;
                g_File.flush();
            }
        }
    }

    void CLog::Abort()
    {
        // 让断言在 Debug 更容易定位；Release 直接 abort
        assert(false && "CLog::Fatal called");
        std::abort();
    }

    const char *CLog::LevelToString(Level level)
    {
        switch (level)
        {
        case Level::Info:
            return "INFO";
        case Level::Warning:
            return "WARN";
        case Level::Error:
            return "ERROR";
        case Level::Fatal:
            return "FATAL";
        default:
            return "UNKNOWN";
        }
    }
} // namespace CM
