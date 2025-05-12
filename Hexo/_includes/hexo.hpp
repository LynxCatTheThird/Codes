#pragma once
#include <fstream> // 文件输入输出
#include <string> // 字符串处理
#include <sstream> // 字符串流处理
#include <iomanip> // 格式化输出
#include <netinet/in.h> // 网络字节序转换
#include <unistd.h> // 系统调用
#include <future> // 异步执行
#include <vector> // 动态数组
#include <algorithm> // 算法
#include <thread> // 线程
#include <chrono> // 时间

#include <utils.hpp> //工具函数

// 函数用途：清理 Hexo 产生的缓存文件
void hexoClean() {
    processPrint("清理 Hexo 缓存文件...");
    std::system("hexo clean > /dev/null");
}

// 函数用途：启动 Hexo 本地预览服务器
void hexoServer() {
    auto totalStart = std::chrono::high_resolution_clock::now(); // 开始记录总执行时间
    hexoClean();
    for (int portNum = 4000; portNum <= 65535; portNum++) {
        std::string portStr = std::to_string(portNum);
        std::string command = "hexo server --port " + portStr + " > /dev/null";
        if (!isPortOpen(portNum)) {
            processPrint("正在尝试于 " + portStr + " 端口启动 Hexo 本地预览服务器...");
            auto serverStart = std::chrono::high_resolution_clock::now(); // 开始记录 hexo server 启动时间
            std::future<int> resultFuture = std::async(std::launch::async, executeCommand, command);

            int portNum = 4000;
            waitWithSpinner("等待 Hexo 本地预览服务器启动...", [&]() {
                return isPortOpen(portNum);
            });

            auto serverEnd = std::chrono::high_resolution_clock::now(); // 结束记录 hexo server 启动时间
            std::chrono::duration<double> serverElapsed = serverEnd - serverStart; // 计算 hexo server 启动时间
            processPrint("Hexo 本地预览服务器启动用时: " + formatDuration(serverElapsed.count(), 3) + " 秒");
            processPrint("Hexo 本地预览服务器已启动于 " + portStr + " 端口。");
            processPrint("您现在可以访问 http://localhost:" + portStr + "/ 预览效果了。");
            break;
        } else {
            errorPrint(portStr + " 端口已被占用，尝试使用下一个端口...");
        }
    }
    auto totalEnd = std::chrono::high_resolution_clock::now(); // 结束记录总执行时间
    std::chrono::duration<double> totalElapsed = totalEnd - totalStart; // 计算总执行时间
    infoPrint("本次操作执行总用时: " + formatDuration(totalElapsed.count(), 3) + " 秒");
}

// 函数用途：部署 Hexo 静态文件
void hexoBuild() {
    auto totalStart = std::chrono::high_resolution_clock::now(); // 开始记录总执行时间
    hexoClean();
    processPrint("生成静态文件...");
    auto generateStart = std::chrono::high_resolution_clock::now(); // 开始记录 Generate 时间
    std::system("hexo generate > /dev/null");
    auto generateEnd = std::chrono::high_resolution_clock::now();  // 结束记录 Generate 时间
    processPrint("执行附属命令...");
    std::size_t additionalToolsRowCount = sizeof(additionalTools) / sizeof(additionalTools[0]);// 获取行数
    auto additionalStart = std::chrono::high_resolution_clock::now();  // 开始记录附属工具时间
    for (unsigned long additionalToolsRunRow = 0; additionalToolsRunRow < additionalToolsRowCount; additionalToolsRunRow++) {
        if (isDependenciesPresent(dependenciesSearchingFile, additionalTools[additionalToolsRunRow][0])) {
            processPrint("正在执行 " + additionalTools[additionalToolsRunRow][0] + " ...");
            std::system(additionalTools[additionalToolsRunRow][1].c_str());
        } else {
            warningPrint("本地项目中未安装 "+additionalTools[additionalToolsRunRow][0] + " 或检索出错，跳过执行。");
        }
    }
    auto additionalEnd = std::chrono::high_resolution_clock::now(); // 结束记录附属工具时间
    processPrint("部署静态文件...");
    std::system("hexo d");
    hexoClean();
    auto totalEnd = std::chrono::high_resolution_clock::now(); // 结束记录总执行时间
    std::chrono::duration<double> generateElapsed = generateEnd - generateStart; // 计算 Generate 时间
    processPrint("生成静态文件用时: " + formatDuration(generateElapsed.count(), 3) + " 秒");
    std::chrono::duration<double> additionalElapsed = additionalEnd - additionalStart; // 计算附属工具时间
    infoPrint("附属工具用时: " + formatDuration(additionalElapsed.count(), 3) + " 秒");
    std::chrono::duration<double> totalElapsed = totalEnd - totalStart; // 计算总执行时间
    infoPrint("本次操作执行总用时: " + formatDuration(totalElapsed.count(), 3) + " 秒");
}