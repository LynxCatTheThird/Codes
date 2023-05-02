// 引入必要的头文件
#include <iostream>
#include <string>
#include <filesystem>
#include <random>
// 使用std命名空间
using namespace std;
// 使用filesystem命名空间
namespace fs = std::filesystem;

// 定义一个函数，用于生成一个任意位数的随机数
int random_number(int digits) {
  // 创建一个随机数生成器
  random_device rd;
  // 创建一个均匀分布
  uniform_int_distribution<int> dist(0, 9);
  // 初始化结果为0
  int result = 0;
  // 循环digits次，每次乘以10并加上一个随机数
  for (int i = 0; i < digits; i++) {
    result = result * 10 + dist(rd);
  }
  // 返回结果
  return result;
}

// 定义一个函数，用于移动指定目录下的所有指定拓展名的文件到另一个目录
void move_files(const string& src_dir, const string& dst_dir, const string& ext) {
  // 创建目标目录，如果已存在则忽略
  fs::create_directories(dst_dir);
  // 遍历源目录下的所有文件
  for (const auto& entry : fs::directory_iterator(src_dir)) {
    // 如果是一个普通文件，并且拓展名匹配
    if (entry.is_regular_file() && entry.path().extension() == "." + ext) {
      // 获取文件的路径
      auto src_path = entry.path();
      // 拼接目标路径
      auto dst_path = dst_dir + "/" + src_path.filename().string();
      // 移动文件
      fs::rename(src_path, dst_path);
      // 打印移动信息
      cout << "Moved " << src_path << " to \"./" << dst_path <<"\""<< endl;
    }
  }
}

// 主函数
int main() {
  // 定义源目录和目标目录
  string src_dir = ".";
  string dst_dir = "bak." + to_string(random_number(3)); // 随机生成三位数的目标目录名
  // 读取用户输入的拓展名
  cout << "请输入要移动的文件的拓展名（不含点）：" << endl;
  string ext;
  cin >> ext;
  // 调用函数，移动文件
  move_files(src_dir, dst_dir, ext);
  // 返回0，表示程序正常结束
  return 0;
}
