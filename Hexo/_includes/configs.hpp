#pragma once

#include <vector>
#include <filesystem>
#include <unordered_map>
#include <unistd.h>

#include <logs.hpp>

namespace SimpleYAML {

    // 最小 YAML 解析器，仅支持 key: value 和 - [k, v] 数组
    class Node {
    public:
        std::unordered_map<std::string, std::string> scalars;
        std::vector<std::pair<std::string, std::string>> additionalTools;

        bool parse(const std::string& content) {
            std::istringstream iss(content);
            std::string line;
            while (std::getline(iss, line)) {
                trim(line);
                if (line.empty() || line[0] == '#') continue;

                if (line[0] == '-') {
                    auto pos1 = line.find('[');
                    auto pos2 = line.find(']');
                    if (pos1 != std::string::npos && pos2 != std::string::npos && pos2 > pos1) {
                        auto pairStr = line.substr(pos1 + 1, pos2 - pos1 - 1);
                        auto comma = pairStr.find(',');
                        if (comma != std::string::npos) {
                            std::string k = pairStr.substr(0, comma);
                            std::string v = pairStr.substr(comma + 1);
                            trim(k); trim(v);
                            stripQuotes(k); stripQuotes(v);
                            additionalTools.emplace_back(k, v);
                        }
                    }
                } else {
                    auto colon = line.find(':');
                    if (colon != std::string::npos) {
                        std::string key = line.substr(0, colon);
                        std::string value = line.substr(colon + 1);
                        trim(key); trim(value);
                        stripQuotes(value);
                        scalars[key] = value;
                    }
                }
            }
            return true;
        }

        std::string get(const std::string& key, const std::string& def = "") const {
            auto it = scalars.find(key);
            return it != scalars.end() ? it->second : def;
        }

        double getDouble(const std::string& key, double def) const {
            try { return std::stod(get(key)); }
            catch (...) { return def; }
        }

        short getShort(const std::string& key, short def) const {
            try { return static_cast<short>(std::stoi(get(key))); }
            catch (...) { return def; }
        }

    private:
        static void trim(std::string& s) {
            size_t start = s.find_first_not_of(" \t\r\n");
            size_t end = s.find_last_not_of(" \t\r\n");
            if (start == std::string::npos) s = "";
            else s = s.substr(start, end - start + 1);
        }

        static void stripQuotes(std::string& s) {
            if ((s.front() == '"' && s.back() == '"') || (s.front() == '\'' && s.back() == '\''))
                s = s.substr(1, s.size() - 2);
        }
    };

} // namespace SimpleYAML

inline std::filesystem::path getExecutableDir() {
#if defined(_WIN32)
    char buffer[MAX_PATH];
    GetModuleFileNameA(NULL, buffer, MAX_PATH);
    return std::filesystem::path(buffer).parent_path();
#elif defined(__APPLE__)
    char buffer[4096];
    uint32_t size = sizeof(buffer);
    if (_NSGetExecutablePath(buffer, &size) == 0)
        return std::filesystem::path(buffer).parent_path();
    else
        return std::filesystem::current_path();
#elif defined(__linux__)
    char buffer[4096];
    ssize_t len = readlink("/proc/self/exe", buffer, sizeof(buffer) - 1);
    if (len != -1) {
        buffer[len] = '\0';
        return std::filesystem::path(buffer).parent_path();
    } else {
        return std::filesystem::current_path();
    }
#else
    return std::filesystem::current_path();
#endif
}

struct Config {
    double similarityThreshold = 0.4;
    std::string dependenciesSearchingFile = "package.json";
    std::vector<std::pair<std::string, std::string>> additionalTools = {
        {"swpp", "hexo swpp"},
        {"gulp", "gulp zip"},
        {"algolia", "hexo algolia"}
    };

    void logPathInfo(const std::filesystem::path& configPath) {
        spdlog::debug("配置文件路径: {}", configPath.string());
        spdlog::debug("可执行文件目录: {}", getExecutableDir().string());
        spdlog::debug("当前工作目录: {}", std::filesystem::current_path().string());
    }

    void loadFromYamlIfExists(const std::string& filename = "config.yaml") {
        std::filesystem::path configPath = getExecutableDir() / filename;

        if (!std::filesystem::exists(configPath)) {
            spdlog::warn("配置文件 {} 未找到，使用默认配置", configPath.string());
            return;
        }

        std::ifstream fin(configPath);
        if (!fin) {
            spdlog::warn("配置文件 {} 无法读取，使用默认值", configPath.string());
            return;
        }

        std::stringstream buffer;
        buffer << fin.rdbuf();
        SimpleYAML::Node node;
        if (!node.parse(buffer.str())) {
            spdlog::error("YAML 配置文件 {} 解析失败", configPath.string());
            return;
        }


        if (similarityThreshold > 0 || similarityThreshold < 1.0) {
            similarityThreshold = node.getDouble("similarityThreshold", similarityThreshold);
        } else {
            spdlog::error("非法相似度阈值，保留默认值 0.4");
        }
        dependenciesSearchingFile = node.get("dependenciesSearchingFile", dependenciesSearchingFile);
        if (!node.additionalTools.empty()) {
            additionalTools = node.additionalTools;
        }

        if (!node.additionalTools.empty()) {
            spdlog::info("加载了 {} 个扩展工具", node.additionalTools.size());
            additionalTools = std::move(node.additionalTools); // 移动语义优化
        }
        spdlog::info("成功加载配置 {}", configPath.string());
    }
};

inline Config config;
