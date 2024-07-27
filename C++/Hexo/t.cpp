#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <sstream>
#include <netinet/in.h>
#include <unistd.h>
#include <future>

// 函数用途：检查文件中是否存在特定依赖项
// 参数：
//   filename: 要检查的文件名
//   dependencie: 要查找的依赖项
// 返回值：若文件中存在指定的依赖项则返回 true，否则返回 false
bool isDependenciesPresent(const std::string& filename, const std::string& dependencie) {
    std::ifstream file(filename); // 打开文件
    if (!file.is_open()) {
        std::cerr << "无法打开" + filename << std::endl; // 输出打开文件失败的信息
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

// 用于异步执行 Hexo 命令
// 输入：const std::string& command - 要执行的命令
// 输出：int - 命令执行的返回值
int executeCommand(const std::string& command) {
    // 调用系统命令执行函数执行给定的命令
    return std::system(command.c_str());
}

void hexoClean() {
    std::cout << "清理中..." << std::endl;
    std::system("hexo clean > /dev/null");
}

void hexoServer() {
    hexoClean();
    for (int portNum = 4000; portNum <= 65535; portNum++) {
        // 将整数转换为字符串
        std::stringstream ss;
        ss << portNum;
        std::string portStr = ss.str();
        // 构造命令字符串 command
        std::string command = "hexo server --port " + portStr + " > /dev/null 2>&1";
        if (!isPortOpen(portNum)) {
            // 创建一个异步任务，使用 executeCommand 函数异步执行 command 命令
            std::cout << "正在尝试于 " + portStr + " 端口启动 Hexo 本地预览服务器..." << std::endl;
            std::future<int> resultFuture = std::async(std::launch::async, executeCommand, command);
            while (true) {
                if (isPortOpen(portNum)) {  // 循环检查端口是否打开
                    std::cout << "Hexo 本地预览服务器已启动于 " + portStr + " 端口" << std::endl;  // 如果端口已打开，则打印服务器已启动消息并结束循环
                    break;
                } else {
                    std::cout << "Hexo 本地预览服务器尚未启动..." << std::endl;  // 如果端口未打开，则打印服务器尚未启动消息
                }
                sleep(2.5);  // 等待 2.5 秒
            }
        } else {
            std::cout << portStr + " 端口已被占用，尝试使用下一个端口..." << std::endl;
        }
    }
}

void hexoBuild() {
    hexoClean();
    std::cout << "生成静态文件..." << std::endl;
    std::system("hexo generate > /dev/null");
    std::cout << "执行性能优化程序..." << std::endl;
    if (isDependenciesPresent("package.json", "swpp")) {
        std::system("hexo swpp");
    } else {
        std::cout << "本地项目中不包含 swpp 或无法判断状态" << std::endl;
    }
    if (isDependenciesPresent("package.json", "gulp")) {
        std::system("gulp zip");
    } else {
        std::cout << "本地项目中不包含 gulp 或无法判断状态" << std::endl;
    }
    std::cout << "部署至平台..." << std::endl;
    std::system("hexo d");
    hexoClean();
}

int main(int argc, char* argv[ ]) {
    if (argc == 1) {
        std::cout << "请输入参数" << std::endl;
        return 1;
    } else if (std::string(argv[1]) == "b") {
        hexoBuild();
    } else if (std::string(argv[1]) == "ut") {
        std::system("git submodule update --remote --merge");
    } else if (std::string(argv[1]) == "u") {
        std::system("ncu -u && npm install");
    } else if (std::string(argv[1]) == "s" || std::string(argv[1]) == "se") {
        hexoServer();
    } else {
        std::cout << "无效的参数" << std::endl;
        return 1;
    }
    return 0;
}
