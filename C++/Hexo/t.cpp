#include <iostream>
#include <cstdlib>
#include <fstream>
using namespace std;

bool isDependenciesPresent(const string& filename, const string& dependencie) {
    ifstream file(filename); // 打开文件
    if (!file.is_open()) {
        cerr << "无法打开"+filename<< endl;
        return false;
    }

    string line;
    while (getline(file, line)) { // 逐行读取文件
        if (line.find(dependencie) != string::npos) { // 查找是否含有预期字符串
            return true;
        }
    }

    return false;
}

void hexoClean(){
  cout<<"Cleaning...\n";
  system("hexo clean > /dev/null");
}

void hexoServer(){
  hexoClean();
  cout<<"Starting...\n";
  system("hexo server > /dev/null");
}

void hexoBuild(){
  hexoClean();
  cout<<"Generating...\n";
  system("hexo generate > /dev/null");
  cout<<"Optimizing...\n";
  if (isDependenciesPresent("package.json", "swpp")){
    system("hexo swpp");
  }else{
    cout << "本地项目中不包含 swpp 或无法判断状态" << endl;
  }
  if (isDependenciesPresent("package.json", "gulp")){
    system("gulp zip");
  }else{
    cout << "本地项目中不包含 gulp 或无法判断状态" << endl;
  }
  cout<<"Deploying...\n";
  system("hexo d");
  hexoClean();
}

int main(int argc, char* argv[]) {
  if (argc == 1) {
    cout << "无效的参数\n";
  } else if (string(argv[1]) == "b") {
    hexoBuild();
  } else if (string(argv[1]) == "ut") {
    system("git submodule update --remote --merge");
  } else if (string(argv[1]) == "u") {
    system("ncu -u && npm install");
  } else if (string(argv[1]) == "s" || string(argv[1]) == "se") {
    hexoServer();
  } else {
    cout << "无效的参数\n";
  }
  return 0;
}
