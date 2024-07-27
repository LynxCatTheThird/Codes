#include <iostream>
#include <cstdlib>
using namespace std;

int main(int argc, char* argv[]) {
  if (argc == 1 || string(argv[1]) == "h") { // 如果没有参数或参数为h
    cout << "帮助信息：\n";
    cout << "t b: 执行hexo cl && hexo g && hexo d && hexo cl\n";
    cout << "t ut: 更新主题\n";
    cout << "t u: 执行ncu -u && npm install\n";
    cout << "t s: 执行hexo cl && hexo g && hexo se\n";
    cout << "t h: 输出帮助信息\n";
  } else if (string(argv[1]) == "b") { // 如果参数为b
    system("hexo cl && hexo g && hexo d && hexo cl"); // 执行系统命令
  } else if (string(argv[1]) == "ut") { // 如果参数为ut
    system("git submodule update --remote --merge"); // 执行系统命令
  } else if (string(argv[1]) == "u") { // 如果参数为u
    system("ncu -u && npm install"); // 执行系统命令
  } else if (string(argv[1]) == "s") { // 如果参数为s
    system("hexo cl && hexo g && hexo se"); // 执行系统命令
  } else {
    cout << "无效的参数\n"; // 其他情况输出错误信息
  }
  return 0;
}
