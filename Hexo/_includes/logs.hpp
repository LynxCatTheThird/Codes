#pragma once

#include <spdlog/spdlog.h> // 日志
#include <spdlog/sinks/stdout_color_sinks.h>

inline void initLogger() {
    auto logger = spdlog::stdout_color_mt("console");
    spdlog::set_default_logger(logger);
    spdlog::set_pattern("%^[%L] %v%$");  // 彩色等级 + 消息
    spdlog::set_level(spdlog::level::trace);  // 打开全部等级
}

// 函数用途：格式化执行用时
// 参数：
//   seconds: 秒数
//   precision: 精度
// 返回值：格式化后的时间字符串
inline std::string formatDuration(double seconds, int precision = 2) {
    std::ostringstream ss;
    ss << std::fixed << std::setprecision(precision) << seconds;
    return ss.str();
}

// 函数用途：输出等待转圈动画，直到条件满足为止
// 参数：
//   label       —— 显示在转圈动画前的标签文字
//   predicate   —— 一个返回 bool 的函数对象；返回 true 表示结束等待
//   interval_ms —— 每次轮询的时间间隔（毫秒），默认为 100ms
// 返回值：无
inline void waitWithSpinner(const std::string& label,
    std::function<bool()> predicate,
    int interval_ms = 100) {
    // 定义转圈动画的字符序列
    static const char spinner[ ] = "|/-\\";
    int count = 0;

    // 当 predicate() 返回 false 时持续等待
    while (!predicate()) {
        // 每隔若干次（可调）输出一个 spinner 字符，避免频繁刷新
        if (count % 10 == 0) {
            int idx = (count / 10) % (sizeof(spinner) - 1);  // 计算当前使用的 spinner 字符索引
            std::cerr << "\r\033[1;33m"  // 输出回车 + 黄色文本（ANSI 转义码）
                << "[W] " << label << " " << spinner[idx]    // 显示标签和 spinner
                << std::flush;                               // 刷新输出
        }

        // 睡眠 interval_ms 毫秒
        std::this_thread::sleep_for(std::chrono::milliseconds(interval_ms));
        ++count;
    }

    // 清除 spinner 行（输出空格覆盖原内容），保持终端整洁
    std::cerr << "\r"
        << std::string(label.size() + 10, ' ')
        << "\033[0m\r" << std::flush;
}
