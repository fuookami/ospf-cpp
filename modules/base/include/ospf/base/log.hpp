#pragma once

/// 日志工具 / Logging utilities
/// 提供最小日志接口。优先使用 spdlog（若可用），否则使用标准输出。

#include <cstdio>
#include <source_location>
#include <string>

namespace ospf::base {

    enum class LogLevel : int {
        Trace = 0,
        Debug = 1,
        Info  = 2,
        Warn  = 3,
        Error = 4,
        Fatal = 5,
        Off   = 6,
    };

    /// 全局日志级别 / Global log level
    inline LogLevel g_log_level = LogLevel::Info;

    inline void set_log_level(LogLevel level) { g_log_level = level; }

    namespace detail {
        inline const char* log_level_str(LogLevel level) {
            switch (level) {
                case LogLevel::Trace: return "TRACE";
                case LogLevel::Debug: return "DEBUG";
                case LogLevel::Info:  return "INFO";
                case LogLevel::Warn:  return "WARN";
                case LogLevel::Error: return "ERROR";
                case LogLevel::Fatal: return "FATAL";
                default: return "OFF";
            }
        }
    }  // namespace detail

    /// 日志输出 / Log output
    inline void log(
        LogLevel level,
        const std::string& message,
        std::source_location loc = std::source_location::current())
    {
        if (level < g_log_level) return;
        std::fprintf(stderr, "[%s] %s (%s:%d)\n",
            detail::log_level_str(level),
            message.c_str(),
            loc.file_name(),
            static_cast<int>(loc.line()));
    }

    inline void trace(const std::string& msg) { log(LogLevel::Trace, msg); }
    inline void debug(const std::string& msg) { log(LogLevel::Debug, msg); }
    inline void info(const std::string& msg)  { log(LogLevel::Info, msg); }
    inline void warn(const std::string& msg)  { log(LogLevel::Warn, msg); }
    inline void error(const std::string& msg) { log(LogLevel::Error, msg); }

}  // namespace ospf::base
