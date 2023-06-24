// 引入必要的头文件
#include <iostream>
#include <string>
#include <filesystem>
#include <cstdlib>
// 使用std命名空间
using namespace std;
// 使用filesystem命名空间
namespace fs = std::filesystem;

// 定义一个函数，用于转换指定目录下的所有指定格式的文件为另一种格式
void convert_files(const string& src_dir, const string& input_format, const string& output_format) {
 // 遍历源目录下的所有文件
 for (const auto& entry : fs::directory_iterator(src_dir)) {
 // 如果是一个普通文件，并且拓展名匹配
 if (entry.is_regular_file() && entry.path().extension() == "." + input_format) {
 // 获取文件的路径
 auto src_path = entry.path();
 // 获取文件名（不含扩展名）
 auto filename = src_path.stem().string();
 // 拼接目标路径
 auto dst_path = src_dir + "/" + filename + "." + output_format;
 // 构造ffmpeg命令，保持视频和音频编码不变
 auto cmd = "ffmpeg -i \"" + src_path.string() + "\" \"" + dst_path + "\"";
 // 调用ffmpeg命令，转换文件格式
 system(cmd.c_str());
 // 打印转换信息
 cout << "Converted " << src_path << " to " << dst_path << endl;
 }
 }
}

// 主函数
int main() {
 // 定义源目录
 string src_dir = ".";
 // 读取用户输入的传入格式和传出格式
 cout << "请输入传入格式（例如：webm）：" << endl;
 string input_format;
 cin >> input_format;
 cout << "请输入传出格式（例如：mp4）：" << endl;
 string output_format;
 cin >> output_format;
 // 调用函数，转换文件格式
 convert_files(src_dir, input_format, output_format);
 // 返回0，表示程序正常结束
 return 0;
}
