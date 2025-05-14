#pragma once

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
