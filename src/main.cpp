#include "log.h"
#include <thread>
#include <chrono>

void tester1()
{
    LOG("测试线程1开始");
    for (int i = 0; i < 5; ++i) {
        LOG("测试线程1: 计数 {}", i);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}


int main()
{
    Logger::instance().set_log_level(Logger::TRACE); // 设置日志级别为TRACE
    Logger::instance().set_console_level(Logger::TRACE);
    Logger::instance().set_log_format("[%H:%M:%S.%e] %v");
    Logger::instance().set_console_format("[%H:%M:%S.%e] %v");
    Logger::instance().init("./logs/sys.log", 15 * 1024 * 1024, 1);

    std::cout << "=== 异步日志系统测试开始 ===" << std::endl;

    // 测试各种日志级别
    LOG("""这是一条普通日志: {}", "hello world");
    LOG_TRACE("这是一条跟踪日志: {}", "trace message");
    LOG_DEBUG("这是一条调试日志: 变量值 = {}", 42);
    LOG_INFO("这是一条信息日志: 操作 {} 已完成", "initialization");
    LOG_WARN("这是一条警告日志: 检测到潜在问题 {}", "memory usage high");
    LOG_ERRO("这是一条错误日志: 错误代码 = {}", 404);
    LOG_CRIT("这是一条严重错误日志: 系统状态 = {}", "critical failure");

    tester1();

    std::cout << "=== 异步日志系统测试结束 ===" << std::endl;
    return 0;
}
