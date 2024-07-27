#include <iostream>
#include <cstdlib>
#include <string>
#include <filesystem>
#include <cstdio> // 加上这个头文件
using namespace std;
using namespace std::filesystem;

int main(int argc, char* argv[]) {
  if (argc == 1 || string(argv[1]) == "h") { // 如果没有参数或参数为h
    cout << "帮助信息：\n";
    cout << "t b: 执行hexo c && hexo g && hexo d && hexo c\n";
    cout << "t ut: 更新主题butterfly\n";
    cout << "t u: 执行ncu -u && npm install\n";
    cout << "t s: 执行hexo cl && hexo se\n";
    cout << "t h: 输出帮助信息\n";
  } else if (string(argv[1]) == "b") { // 如果参数为b
    system("hexo cl && hexo g && hexo d && hexo cl"); // 执行系统命令
  } else if (string(argv[1]) == "ut") { // 如果参数为ut
    path git_up = "themes/butterfly/.git.up"; // 定义路径对象
    path git = "themes/butterfly/.git";
    if (exists(git_up)) { // 如果.git.up存在
      std::rename(git_up.c_str(), git.c_str()); // 用std::rename函数重命名为.git
      system("cd themes/butterfly && git pull"); // 执行系统命令
      std::rename(git.c_str(), git_up.c_str()); // 用std::rename函数重命名回.git.up
    } else {
      cout << ".git.up不存在\n";
    }
  } else if (string(argv[1]) == "u") { // 如果参数为u
    system("ncu -u && npm install"); // 执行系统命令
  } else if (string(argv[1]) == "s") { // 如果参数为s
    system("hexo cl && hexo se"); // 执行系统命令
  } else {
    cout << "无效的参数\n"; // 其他情况输出错误信息
  }
  return 0;
}
