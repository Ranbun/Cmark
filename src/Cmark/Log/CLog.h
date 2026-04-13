#ifndef CAMERAMARK_CLOG_H
#define CAMERAMARK_CLOG_H

#include <cassert>
#include <filesystem>
#include <mutex>
#include <sstream>
#include <string>
#include <string_view>
#include <type_traits>
#include <typeinfo>
#include <utility>

#if defined(__has_include)
#if __has_include(<QString>)
#include <QString>
#define CM_HAS_QSTRING 1
#endif
#endif

namespace CM
{
    class CLog
    {
    public:
        static void Init();

        static void SetLogFilePath(std::filesystem::path path);
        static std::filesystem::path GetLogFilePath();

        static void EnableConsole(bool enabled);
        static void EnableFile(bool enabled);

        template <typename T>
        static void PrintMes(const T &value)
        {
            Info(value);
        }

        template <typename T>
        static void Info(const T &value)
        {
            Log(Level::Info, value);
        }

        template <typename T>
        static void Warning(const T &value)
        {
            Log(Level::Warning, value);
        }

        template <typename T>
        static void Error(const T &value)
        {
            Log(Level::Error, value);
        }

        static void Error(const std::string &value)
        {
            Log(Level::Error, value);
        }

        static void Error(std::string_view value)
        {
            Log(Level::Error, value);
        }

        static void Error(const char *value)
        {
            Log(Level::Error, value);
        }

        template <typename T>
        static void Fatal(const T &value)
        {
            Log(Level::Fatal, value);
            Abort();
        }

        static void Fatal(const std::string &value)
        {
            Log(Level::Fatal, value);
            Abort();
        }

        static void Fatal(std::string_view value)
        {
            Log(Level::Fatal, value);
            Abort();
        }

        static void Fatal(const char *value)
        {
            Log(Level::Fatal, value);
            Abort();
        }

    private:
        enum class Level
        {
            Info,
            Warning,
            Error,
            Fatal
        };

        static std::once_flag m_InitFlag;

        static void EnsureInit();
        static void Write(Level level, std::string_view message);
        static void Abort();
        static const char *LevelToString(Level level);

        template <typename T>
        static void Log(Level level, const T &value)
        {
            EnsureInit();
            const auto s = ToString(value);
            Write(level, s);
        }

        template <typename T, typename = void>
        struct IsStreamable : std::false_type
        {
        };

        template <typename T>
        struct IsStreamable<T, std::void_t<decltype(std::declval<std::ostream &>() << std::declval<const T &>())>>
            : std::true_type
        {
        };

        static std::string ToString(std::string_view v) { return std::string(v); }
        static std::string ToString(const std::string &v) { return v; }
        static std::string ToString(const char *v) { return v ? std::string(v) : std::string(); }

#if defined(CM_HAS_QSTRING)
        static std::string ToString(const QString &v) { return v.toStdString(); }
#endif

        template <typename T>
        static std::string ToString(const T &v)
        {
            if constexpr (std::is_same_v<std::decay_t<T>, std::filesystem::path>)
            {
                return v.string();
            }
            else if constexpr (IsStreamable<T>::value)
            {
                std::ostringstream oss;
                oss << v;
                return oss.str();
            }
            else
            {
                return std::string("{unprintable: ") + typeid(T).name() + "}";
            }
        }
    };

    inline CLog g_Log;
#define CLogInstance CM::g_Log

} // CM

#endif // CAMERAMARK_CLOG_H
