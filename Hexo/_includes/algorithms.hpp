#pragma once
#include <vector> // 动态数组

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
