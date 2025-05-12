#include <iostream> //输入输出

#include <_configs.hpp> //配置文件
#include <utils.hpp> //辅助函数
#include <logs.hpp> //日志
#include <algorithms.hpp> //辅助算法
#include <hexo.hpp> //主体

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

// 函数用途：显示帮助信息
void utilHelper() {
    std::cout << "Hexo 辅助工具\n" <<  "\
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
    initLogger();

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
