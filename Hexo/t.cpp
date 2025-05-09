#include <iostream> // 输入输出
#include <cstdlib> // 系统命令
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

// Configs:
double SIMILARITY_THRESHOLD = 0.4; // 字符串相似度阈值
short logLevel = 2;// 输出等级，数字越大越详细
std::string dependenciesSearchingFile = "package.json"; // 依赖搜索文件
std::string additionalTools[3][2] = {
    // 格式：{查询关键字, 工具命令}
    {"swpp","hexo swpp"},
    {"gulp","gulp zip"},
    {"algolia","hexo algolia"}
};
// 颜色定义
#define RESET   "\033[0m"
#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */
#define WHITE   "\033[37m"      /* White */
#define BOLDBLACK   "\033[1m\033[30m"      /* Bold Black */
#define BOLDRED     "\033[1m\033[31m"      /* Bold Red */
#define BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */
#define BOLDYELLOW  "\033[1m\033[33m"      /* Bold Yellow */
#define BOLDBLUE    "\033[1m\033[34m"      /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m"      /* Bold Magenta */
#define BOLDCYAN    "\033[1m\033[36m"      /* Bold Cyan */
#define BOLDWHITE   "\033[1m\033[37m"      /* Bold White */

// 函数用途：打印动作信息（-1 级）
// 参数：
//   message: 要打印的动作信息
void processPrint(const std::string& message) {
    if (logLevel >= -1) {
        std::cout << BLUE << "[进程] " << RESET << message << std::endl;
    }
}

// 函数用途：打印错误信息（0 级）
// 参数：
//   message: 要打印的错误信息
void errorPrint(const std::string& message) {
    if (logLevel >= 0) {
        std::cerr << BOLDRED << "[错误] " << RESET << message << std::endl;
    }
}

// 函数用途：打印警告信息（1 级）
// 参数：
//   message: 要打印的警告信息
void warningPrint(const std::string& message) {
    if (logLevel >= 1) {
        std::cerr << YELLOW << "[警告] " << RESET << message << std::endl;
    }
}

// 函数用途：打印拓展信息（2 级）
// 参数：
//   message: 要打印的调试信息
void infoPrint(const std::string& message) {
    if (logLevel >= 2) {
        std::cout << GREEN << "[信息] " << RESET << message << std::endl;
    }
}

// 函数用途：打印调试信息（3 级）
// 参数：
//   message: 要打印的调试信息
void debugPrint(const std::string& message) {
    if (logLevel >= 3) {
        std::cout << MAGENTA << "[调试] " << RESET << message << std::endl;
    }
}

// 函数用途：格式化执行用时
// 参数：
//   seconds: 秒数
//   precision: 精度
// 返回值：格式化后的时间字符串
std::string formatDuration(double seconds, int precision) {
    std::stringstream ss;
    ss << std::fixed << std::setprecision(precision) << seconds;
    return ss.str();
}

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
        infoPrint("子串方法判断成功，识别意图为 " + s1);
        return true;
    } else if (isSimilar(s1, s2, SIMILARITY_THRESHOLD)) {
        infoPrint("相似度方法判断成功，识别意图为 " + s1);
        return true;
    } else {
        debugPrint("判断意图是否为 " + s1 + " 失败");
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
        errorPrint("无法打开" + filename); // 输出打开文件失败的信息
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

            if (logLevel >= 1) {
                int checkCount = 0;
                std::string waitingChars = "|/-\\"; // 等待动画
                while (!isPortOpen(portNum)) {
                    if (checkCount % 10 == 0) {  // 每10次检查更新一次输出
                        std::cerr << "\r\033[33m" << "[警告] " << RESET << "等待 Hexo 本地预览服务器启动... " << waitingChars[checkCount / 10 % 4] << std::flush; //这里不够优雅
                    }
                    std::this_thread::sleep_for(std::chrono::milliseconds(100));
                    checkCount++;
                }
                std::cout << std::endl;  // 完成后换行
            }

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

// 函数用途：显示帮助信息
void utilHelper() {
    std::cout << BOLDMAGENTA << "Hexo 辅助工具\n" << RESET << "\
1. 部署静态文件：build、deploy\n\
2. 启动本地预览服务器：server\n\
3. 更新依赖包：packages\n\
4. 更新子模块（更新主题）：theme\n\
5. 显示帮助信息：help\n\
命令有一定鲁棒性，欢迎翻阅源代码查看具体实现\n";
}

// 函数用途：主函数
// 参数：
//   argc: 命令行参数个数
//   argv: 命令行参数数组
// 返回值：0 - 成功，1 - 失败
int main(int argc, char* argv[ ]) {
    if (argc == 1) {
        errorPrint("请输入参数");
        utilHelper();
        return 1;
    } else if (isOrder("build", argv[1]) || isOrder("deploy", argv[1])) {
        hexoBuild();
    } else if (isOrder("server", argv[1])) {
        hexoServer();
    } else if (isOrder("theme", argv[1])) {
        std::system("git submodule update --remote --merge");
    } else if (isOrder("packages", argv[1])) {
        std::system("rm -rf package-lock.json && rm -rf node_modules/ && ncu -u && npm install");
    } else {
        errorPrint("无效的参数：所有判断都失败了，无法判断命令意图。");
        errorPrint("您确定 " + std::string(argv[1]) + " 是正确的命令吗？");
        utilHelper();
        return 1;
    }
    return 0;
}
