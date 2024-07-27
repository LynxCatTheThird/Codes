#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>

bool isDependenciesPresent(const std::string& filename, const std::string& dependencie) {
    std::ifstream file(filename); // 打开文件
    if (!file.is_open()) {
        std::cerr << "无法打开" + filename << std::endl;
        return false;
    }

    std::string line;
    while (std::getline(file, line)) { // 逐行读取文件
        if (line.find(dependencie) != std::string::npos) { // 查找是否含有预期字符串
            return true;
        }
    }

    return false;
}

void hexoClean() {
    std::cout << "Cleaning...\n";
    std::system("hexo clean > /dev/null");
}

void hexoServer() {
    hexoClean();
    std::cout << "Starting...\n";
    std::system("hexo server > /dev/null");
}

void hexoBuild() {
    hexoClean();
    std::cout << "Generating...\n";
    std::system("hexo generate > /dev/null");
    std::cout << "Optimizing...\n";
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
    std::cout << "Deploying...\n";
    std::system("hexo d");
    hexoClean();
}

int main(int argc, char* argv[]) {
    if (argc == 1) {
        std::cout << "无效的参数\n";
    } else if (std::string(argv[1]) == "b") {
        hexoBuild();
    } else if (std::string(argv[1]) == "ut") {
        std::system("git submodule update --remote --merge");
    } else if (std::string(argv[1]) == "u") {
        std::system("ncu -u && npm install");
    } else if (std::string(argv[1]) == "s" || std::string(argv[1]) == "se") {
        hexoServer();
    } else {
        std::cout << "无效的参数\n";
    }
    return 0;
}
