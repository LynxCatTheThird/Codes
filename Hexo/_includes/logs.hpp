// #include <iostream> // 输入输出
// #include <string> // 字符串处理
// #include <sstream> // 字符串流处理
// #include <iomanip> // 格式化输出

// #include <_configs.hpp>

// // 颜色定义
// #define RESET   "\033[0m"
// #define BLACK   "\033[30m"      /* Black */
// #define RED     "\033[31m"      /* Red */
// #define GREEN   "\033[32m"      /* Green */
// #define YELLOW  "\033[33m"      /* Yellow */
// #define BLUE    "\033[34m"      /* Blue */
// #define MAGENTA "\033[35m"      /* Magenta */
// #define CYAN    "\033[36m"      /* Cyan */
// #define WHITE   "\033[37m"      /* White */
// #define BOLDBLACK   "\033[1m\033[30m"      /* Bold Black */
// #define BOLDRED     "\033[1m\033[31m"      /* Bold Red */
// #define BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */
// #define BOLDYELLOW  "\033[1m\033[33m"      /* Bold Yellow */
// #define BOLDBLUE    "\033[1m\033[34m"      /* Bold Blue */
// #define BOLDMAGENTA "\033[1m\033[35m"      /* Bold Magenta */
// #define BOLDCYAN    "\033[1m\033[36m"      /* Bold Cyan */
// #define BOLDWHITE   "\033[1m\033[37m"      /* Bold White */

// // 函数用途：打印动作信息（-1 级）
// // 参数：
// //   message: 要打印的动作信息
// void processPrint(const std::string& message) {
//     if (logLevel >= -1) {
//         std::cout << BLUE << "[进程] " << RESET << message << std::endl;
//     }
// }

// // 函数用途：打印错误信息（0 级）
// // 参数：
// //   message: 要打印的错误信息
// void errorPrint(const std::string& message) {
//     if (logLevel >= 0) {
//         std::cerr << BOLDRED << "[错误] " << RESET << message << std::endl;
//     }
// }

// // 函数用途：打印警告信息（1 级）
// // 参数：
// //   message: 要打印的警告信息
// void warningPrint(const std::string& message) {
//     if (logLevel >= 1) {
//         std::cerr << YELLOW << "[警告] " << RESET << message << std::endl;
//     }
// }

// // 函数用途：打印拓展信息（2 级）
// // 参数：
// //   message: 要打印的调试信息
// void infoPrint(const std::string& message) {
//     if (logLevel >= 2) {
//         std::cout << GREEN << "[信息] " << RESET << message << std::endl;
//     }
// }

// // 函数用途：打印调试信息（3 级）
// // 参数：
// //   message: 要打印的调试信息
// void debugPrint(const std::string& message) {
//     if (logLevel >= 3) {
//         std::cout << MAGENTA << "[调试] " << RESET << message << std::endl;
//     }
// }

// // 函数用途：格式化执行用时
// // 参数：
// //   seconds: 秒数
// //   precision: 精度
// // 返回值：格式化后的时间字符串
// std::string formatDuration(double seconds, int precision) {
//     std::stringstream ss;
//     ss << std::fixed << std::setprecision(precision) << seconds;
//     return ss.str();
// }


#pragma once
#include <string>
#include <sstream>
#include <iomanip>
#include <memory>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

inline void initLogger() {
    auto logger = spdlog::stdout_color_mt("console");
    spdlog::set_default_logger(logger);
    spdlog::set_pattern("%^[%L] %v%$");  // 彩色等级 + 消息
    spdlog::set_level(spdlog::level::trace);  // 打开全部等级
}

inline void processPrint(const std::string& message) {
    if (logLevel >= -1) spdlog::info(message);
}

inline void errorPrint(const std::string& message) {
    if (logLevel >= 0) spdlog::error(message);
}

inline void warningPrint(const std::string& message) {
    if (logLevel >= 1) spdlog::warn(message);
}

inline void infoPrint(const std::string& message) {
    if (logLevel >= 2) spdlog::info(message);
}

inline void debugPrint(const std::string& message) {
    if (logLevel >= 3) spdlog::debug(message);
}

inline std::string formatDuration(double seconds, int precision = 2) {
    std::ostringstream ss;
    ss << std::fixed << std::setprecision(precision) << seconds;
    return ss.str();
}

inline void waitWithSpinner(const std::string& label,
    std::function<bool()> predicate,
    int interval_ms = 100) {
    if (logLevel < 1) return;

    static const char spinner[ ] = "|/-\\";
    int count = 0;
    while (!predicate()) {
        if (count % 10 == 0) {
            int idx = (count / 10) % (sizeof(spinner) - 1);
            std::cerr << "\r\033[33m"  // 黄色
                << "[W] " << label << " " << spinner[idx]
                << std::flush;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(interval_ms));
        ++count;
    }
    // 清除 spinner 行，保持终端干净
    std::cerr << "\r"
        << std::string(label.size() + 10, ' ')
        << "\r" << std::flush;
}
