#pragma once

#include <netinet/in.h> // 网络字节序转换
#include <unistd.h> // 系统调用

#include <logs.hpp> //日志

// 函数用途：检查文件中是否存在特定依赖项
// 参数：
//   filename: 要检查的文件名
//   dependencie: 要查找的依赖项
// 返回值：若文件中存在指定的依赖项则返回 true，否则返回 false
bool isDependenciesPresent(const std::string& filename, const std::string& dependencie) {
    std::ifstream file(filename); // 打开文件
    if (!file.is_open()) {
        spdlog::error("无法打开" + filename); // 输出打开文件失败的信息
        return false;
    }
    std::string line;
    while (std::getline(file, line)) { // 逐行读取文件
        if (line.find(dependencie) != std::string::npos) { // 查找是否含有预期字符串
            return true; // 若找到依赖项则返回 true
        }
    }
    return false; // 未找到依赖项则返回 false
}

// 函数用途：判断给定端口是否处于打开状态
// 参数：port - 待判断的端口号
// 返回值：若端口打开则返回true，否则返回false
bool isPortOpen(int port) {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0); // 创建一个套接字
    if (sockfd < 0) { // 若创建套接字失败
        perror("socket"); // 输出错误信息
        return false; // 返回端口未打开
    }
    struct sockaddr_in addr;
    addr.sin_family = AF_INET; // 地址族为IPv4
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK); // 设置为本地回环地址
    addr.sin_port = htons(port); // 设置端口号
    int result = connect(sockfd, (struct sockaddr*)&addr, sizeof(addr)); // 尝试连接到指定端口
    close(sockfd); // 关闭套接字
    if (result == 0) {
        return true; // 端口打开
    } else {
        return false; // 端口未打开
    }
}

// 用于异步执行系统命令
// 输入：const std::string& command - 要执行的命令
// 输出：int - 命令执行的返回值
int executeCommand(const std::string& command) {
    // 调用系统命令执行函数执行给定的命令
    return std::system(command.c_str());
}
