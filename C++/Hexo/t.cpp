#include <iostream> // 输入输出
#include <cstdlib> // 系统命令
#include <fstream> // 文件输入输出
#include <string> // 字符串处理
#include <sstream> // 字符串流处理
#include <netinet/in.h> // 网络字节序转换
#include <unistd.h> // 系统调用
#include <future> // 异步执行
#include <vector> // 动态数组
#include <algorithm> // 算法

// Configs:
double SIMILARITY_THRESHOLD = 0.4; // 字符串相似度阈值
bool displayErrorPrompt = 0;// 是否输出判断失败提示

// 函数用途：计算两个字符串之间的 Levenshtein 距离
// 参数：
//   s1: 字符串1
//   s2: 字符串2
// 返回值：两个字符串之间的 Levenshtein 距离
// 算法：动态规划
int levenshteinDistance(const std::string& s1, const std::string& s2) {
    // 初始化字符串长度
    int len1 = s1.size();
    int len2 = s2.size();
    // 创建二维DP数组
    std::vector<std::vector<int>> dp(len1 + 1, std::vector<int>(len2 + 1, 0));

    // 初始化边界条件
    for (int i = 0; i <= len1; ++i) {
        dp[i][0] = i;
    }
    for (int j = 0; j <= len2; ++j) {
        dp[0][j] = j;
    }

    // 填充DP数组
    for (int i = 1; i <= len1; ++i) {
        for (int j = 1; j <= len2; ++j) {
            // 根据字符是否相等更新DP数组
            if (s1[i - 1] == s2[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1];
            } else {
                dp[i][j] = std::min({ dp[i - 1][j - 1], dp[i - 1][j], dp[i][j - 1] }) + 1;
            }
        }
    }

    // 返回Levenshtein距离
    return dp[len1][len2];
}


// 函数用途：计算两个字符串之间的字符串相似度
// 参数：
//   s1: 字符串1
//   s2: 字符串2
// 返回值：两个字符串之间的字符串相似度
// 算法：Levenshtein 距离与字符串长度的比值
double stringSimilarity(const std::string& s1, const std::string& s2) {
    // 计算Levenshtein距离
    int dist = levenshteinDistance(s1, s2);
    // 计算字符串最大长度
    int maxLen = std::max(s1.size(), s2.size());
    // 计算并返回字符串相似度
    return 1.0 - static_cast<double>(dist) / maxLen;
}

// 函数用途：判断第二个字符串是否为第一个字符串的头部子串
// 参数：
//   s1: 字符串1
//   s2: 字符串2
// 返回值：若第二个字符串为第一个字符串的开头一部分则返回true,否则为false 
// 举例：
//   s1 = "hello", s2 = "hello world"
//   isSubString(s1, s2) 返回 true
//   s1 = "world", s2 = "hello world"
//   isSubString(s1, s2) 返回 false
bool isSubString(const std::string& s1, const std::string& s2) {
    return s2.find(s1) == 0;
}

// 函数用途：判断两个字符串相似度是否满足阈值
// 参数：
//   s1: 字符串1
//   s2: 字符串2
//   threshold: 相似度阈值
// 返回值：若两个字符串相似度大于等于阈值则返回 true，否则返回 false
bool isSimilar(const std::string& s1, const std::string& s2, double threshold) {
    return stringSimilarity(s1, s2) >= threshold;
}

// 函数用途：判断命令意图
// 参数：
//   s1: 传入预期的命令
//   s2: 传入argv[1]
// 返回值：若匹配成功则返回 true，否则返回 false
bool isOrder(const std::string& s1, const std::string& s2) {
    if (isSubString(s1, s2) || isSubString(s2, s1)) {
        std::cout << "子串方法判断成功，识别意图为 " << s1 << std::endl;
        return true;
    } else if (isSimilar(s1, s2, SIMILARITY_THRESHOLD)) {
        std::cout << "相似度方法判断成功，识别意图为 " << s1 << std::endl;
        return true;
    } else {
        if (displayErrorPrompt) {
            std::cout << "判断意图不是 " << s1 << std::endl;
        }
        return false;
    }
}

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

// 用于异步执行系统命令
// 输入：const std::string& command - 要执行的命令
// 输出：int - 命令执行的返回值
int executeCommand(const std::string& command) {
    // 调用系统命令执行函数执行给定的命令
    return std::system(command.c_str());
}

// 函数用途：清理 Hexo 产生的缓存文件
void hexoClean() {
    std::cout << "清理中..." << std::endl;
    std::system("hexo clean > /dev/null");
}

// 函数用途：启动 Hexo 本地预览服务器
void hexoServer() {
    hexoClean();
    for (int portNum = 4000; portNum <= 65535; portNum++) {
        // 将整数转换为字符串
        std::stringstream ss;
        ss << portNum;
        std::string portStr = ss.str();
        // 构造命令字符串 command
        std::string command = "hexo server --port " + portStr + " > /dev/null";
        if (!isPortOpen(portNum)) {
            // 创建一个异步任务，使用 executeCommand 函数异步执行 command 命令
            std::cout << "正在尝试于 " + portStr + " 端口启动 Hexo 本地预览服务器..." << std::endl;
            std::future<int> resultFuture = std::async(std::launch::async, executeCommand, command);
            while (!isPortOpen(portNum)) {// 循环检查端口是否打开
                std::cout << "Hexo 本地预览服务器尚未启动..." << std::endl;  // 如果端口未打开，则打印服务器尚未启动消息
                sleep(2);  // 等待 2.5 秒
            }
            std::cout << "Hexo 本地预览服务器已启动于 " + portStr + " 端口" << std::endl;  // 如果端口已打开，则打印服务器已启动消息并结束循环
            break;
        } else {
            std::cout << portStr + " 端口已被占用，尝试使用下一个端口..." << std::endl;
        }
    }
}

// 函数用途：部署 Hexo 静态文件
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

// 函数用途：显示帮助信息
void utilHelper() {
    std::cout << "Hexo 辅助工具" << std::endl;
    std::cout << "1. 部署静态文件：build、deploy" << std::endl;
    std::cout << "2. 启动本地预览服务器：server" << std::endl;
    std::cout << "3. 更新依赖包：updatepackages、update" << std::endl;
    std::cout << "4. 更新子模块（更新主题）：updatesubmodules、updatetheme" << std::endl;
    std::cout << "5. 显示帮助信息：help" << std::endl;
    std::cout << "命令有一定鲁棒性，欢迎翻阅源代码查看具体实现" << std::endl;
}

// 函数用途：主函数
// 参数：
//   argc: 命令行参数个数
//   argv: 命令行参数数组
// 返回值：0 - 成功，1 - 失败
int main(int argc, char* argv[ ]) {
    if (argc == 1) {
        std::cout << "请输入参数" << std::endl;
        utilHelper();
        return 1;
    } else if (isOrder("build", argv[1])) {
        hexoBuild();
    } else if (isOrder("updatesubmodules", argv[1]) || isOrder("updatetheme", argv[1])) {
        std::system("git submodule update --remote --merge");
    } else if (isOrder("updatepackages", argv[1]) || isOrder("update", argv[1])) {
        std::system("ncu -u && npm install");
    } else if (isOrder("server", argv[1])) {
        hexoServer();
    } else if (isOrder("deploy", argv[1])) {
        utilHelper();
    }
    else {
        std::cout << "无效的参数：所有判断都失败了，无法判断命令意图" << std::endl;
        utilHelper();
        return 1;
    }
    return 0;
}
