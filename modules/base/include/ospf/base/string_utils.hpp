#pragma once

/// 字符串工具 / String utilities

#include <algorithm>
#include <sstream>
#include <string>
#include <vector>

namespace ospf::base {

    /// 分割字符串 / Split string by delimiter
    [[nodiscard]] inline std::vector<std::string> split(
        const std::string& str, char delimiter)
    {
        std::vector<std::string> tokens;
        std::istringstream iss(str);
        std::string token;
        while (std::getline(iss, token, delimiter)) {
            tokens.push_back(token);
        }
        return tokens;
    }

    /// 连接字符串 / Join strings with separator
    [[nodiscard]] inline std::string join(
        const std::vector<std::string>& parts, const std::string& separator)
    {
        if (parts.empty()) return {};
        std::string result = parts[0];
        for (std::size_t i = 1; i < parts.size(); ++i) {
            result += separator + parts[i];
        }
        return result;
    }

    /// 修剪空白 / Trim whitespace
    [[nodiscard]] inline std::string trim(const std::string& str) {
        auto start = str.find_first_not_of(" \t\n\r");
        if (start == std::string::npos) return {};
        auto end = str.find_last_not_of(" \t\n\r");
        return str.substr(start, end - start + 1);
    }

    /// 转小写 / To lowercase
    [[nodiscard]] inline std::string to_lower(const std::string& str) {
        std::string result = str;
        std::transform(result.begin(), result.end(), result.begin(),
            [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
        return result;
    }

    /// 转大写 / To uppercase
    [[nodiscard]] inline std::string to_upper(const std::string& str) {
        std::string result = str;
        std::transform(result.begin(), result.end(), result.begin(),
            [](unsigned char c) { return static_cast<char>(std::toupper(c)); });
        return result;
    }

    /// 是否以 prefix 开头 / Starts with
    [[nodiscard]] inline bool starts_with(
        const std::string& str, const std::string& prefix)
    {
        if (prefix.size() > str.size()) return false;
        return str.compare(0, prefix.size(), prefix) == 0;
    }

    /// 是否以 suffix 结尾 / Ends with
    [[nodiscard]] inline bool ends_with(
        const std::string& str, const std::string& suffix)
    {
        if (suffix.size() > str.size()) return false;
        return str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
    }

}  // namespace ospf::base
