#include "log.h"

Logger::Logger(){}
    
Logger::~Logger() {
    // spdlog::shutdown();
}

Logger& Logger::instance()
{
    static Logger inst; // 静态局部变量，确保只创建一次
    return inst;
}

void Logger::set_log_level(LOG_TYPE level)
{
    if(level < Logger::TRACE || level > Logger::OFF)
    {
        level = Logger::TRACE; // 如果传入的日志等级不在范围内，默认使用trace级别
        std::cout << "无效的日志等级，默认使用trace级别\n";
    }
    m_log_level = static_cast<spdlog::level::level_enum>(level);
}

void Logger::set_console_level(LOG_TYPE level)
{
    if(level < Logger::TRACE || level > Logger::OFF)
    {
        level = Logger::TRACE; // 如果传入的日志等级不在范围内，默认使用trace级别
        std::cout << "无效的控制台日志等级，默认使用trace级别\n";
    }
    m_console_level = static_cast<spdlog::level::level_enum>(level);
}

void Logger::init(const std::string& filename, 
                        std::size_t max_size, 
                        std::size_t max_files)
{
    // 1. 创建异步线程池
    spdlog::init_thread_pool(8192, 1); // 队列大小为8192，线程数为1

    /** 2. 创建文件日志接收器
     *  创建一个旋转文件日志接收器，文件名为filename，
     *  每个日志文件的最大大小为10MB，
     *  最多保留1个备份文件。
     */
    auto file_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
        filename, max_size, max_files);
    file_sink->set_level(m_log_level);
    
    // 控制台日志接收器
    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    if (m_OpenConsole) {
        // 3. 添加控制台接收器
        console_sink->set_level(m_console_level);
    } else {
        console_sink->set_level(spdlog::level::off);
    }

    // 3. 创建格式化器
    file_sink->set_pattern(m_log_format);    // 设置文件日志格式 
    console_sink->set_pattern(m_console_format); // 设置控制台日志格式
    
    // 4. 创建异步日志记录器
    logger_ = std::make_shared<spdlog::async_logger>(
        "async_logger",
        spdlog::sinks_init_list{file_sink, console_sink},
        spdlog::thread_pool(),
        spdlog::async_overflow_policy::block
    );

    // 5. 设置日志级别为所有级别
    logger_->set_level(spdlog::level::trace);
    
    // 6. 设置为全局记录器
    spdlog::set_default_logger(logger_);

    spdlog::flush_every(std::chrono::seconds(1));
    spdlog::flush_on(spdlog::level::debug);
}