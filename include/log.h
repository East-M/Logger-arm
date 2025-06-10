#ifndef LOG_H
#define LOG_H

#include <iostream>
#include "spdlog/spdlog.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/async.h"

#define FILEPATH "./logs/debug.log" // 日志文件路径
#define MAX_LOG_SIZE 15728640 // 日志文件最大大小 5MB
#define MAX_LOG_FILES 1 // 日志文件最大数量

/************************** 宏定义 **************************/
// 默认日志宏
#define LOG(...) \
    Logger::instance().log(__VA_ARGS__)

// 分级日志宏
#define LOG_TRACE(...) \
    Logger::instance().log(Logger::TRACE, __VA_ARGS__)
#define LOG_DEBUG(...) \
    Logger::instance().log(Logger::DEBUG, __VA_ARGS__)
#define LOG_INFO(...) \
    Logger::instance().log(Logger::INFO, __VA_ARGS__)
#define LOG_WARN(...) \
    Logger::instance().log(Logger::WARN, __VA_ARGS__)
#define LOG_ERRO(...) \
    Logger::instance().log(Logger::ERRO, __VA_ARGS__)
#define LOG_CRIT(...) \
    Logger::instance().log(Logger::CRITICAL, __VA_ARGS__)
#define LOG_OFF(...) \
    Logger::instance().log(Logger::OFF, __VA_ARGS__)

/**********************************************************/

class Logger
{
public:
    enum LOG_TYPE{
        TRACE = spdlog::level::trace,
        DEBUG = spdlog::level::debug,
        INFO = spdlog::level::info,
        WARN = spdlog::level::warn,
        ERRO = spdlog::level::err,
        CRITICAL = spdlog::level::critical,
        OFF = spdlog::level::off
    };

    Logger();       
    ~Logger();

    // 添加instance()静态函数
    static Logger& instance();

    // 设置日志等级
    void set_log_level(LOG_TYPE level);
    void set_console_level(LOG_TYPE level);

    // 设置日志格式
    void set_log_format(const std::string& format){m_log_format = format;}
    void set_console_format(const std::string& format){m_console_format = format;}

    // 初始化日志系统
    void init(const std::string& filename = FILEPATH,        // 日志文件名
                    std::size_t max_size = MAX_LOG_SIZE,  // 日志的最大大小
                    std::size_t max_files = MAX_LOG_FILES// 日志文件的最大数量
                    );

    // 支持格式化字符串的日志函数
    template <typename... Args>
    void log(LOG_TYPE level, fmt::format_string<Args...> fmt, Args&&... args);
    template <typename... Args>
    void log(fmt::format_string<Args...> fmt, Args&&... args);

private:
    bool m_OpenConsole = {true};
    spdlog::level::level_enum m_log_level = spdlog::level::trace; // 默认日志级别为trace
    spdlog::level::level_enum m_console_level = spdlog::level::trace; // 控制台日志级别
    std::string m_log_format = "[%Y-%m-%d %H:%M:%S.%e] %^[%l] [thread %t] %v%$"; // 日志格式
    std::string m_console_format = "[%Y-%m-%d %H:%M:%S.%e] %^[%l] [thread %t] %v%$"; // 控制台日志格式
    std::shared_ptr<spdlog::logger> logger_; // 使用基类指针
};
    
template <typename... Args>
void Logger::log(LOG_TYPE level, fmt::format_string<Args...> fmt, Args&&... args)
{
    if (!logger_) return;

    // 格式化消息
    std::string formatted_message;
    // 使用fmt库进行格式化
    formatted_message = fmt::format(fmt, std::forward<Args>(args)...);

    // 保存日志
    switch(level)
    {
    case spdlog::level::trace:
    {
        logger_->trace("[trace] {}", formatted_message);
        break;
    }
    case spdlog::level::debug:
    {
        logger_->debug("[debug] {}", formatted_message);
        break;
    }
    case spdlog::level::info:
    {
        logger_->info("[info] {}", formatted_message);
        break;
    }
    case spdlog::level::warn:
    {
        logger_->warn("[warning] {}", formatted_message);
        break;
    }
    case spdlog::level::err:
    {
        logger_->error("[error] {}", formatted_message);
        break;
    }
    case spdlog::level::critical:
    {
        logger_->critical("[critical] {}", formatted_message);
        break;
    }
    case spdlog::level::off:
        // 不记录off级别的日志
        break;
    default:
    {
        logger_->trace("{}", formatted_message);
        break;
    }
    }
}

template <typename... Args>
void Logger::log(fmt::format_string<Args...> fmt, Args&&... args)
{
    if (!logger_) return;

    // 格式化消息
    std::string formatted_message;
    // 使用fmt库进行格式化
    formatted_message = fmt::format(fmt, std::forward<Args>(args)...);
    logger_->trace("{}", formatted_message);
}

#endif