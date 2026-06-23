#pragma once

/// CSV 序列化 / CSV serialization

#include <sstream>
#include <string>
#include <vector>

namespace ospf::base {

    /// CSV 行解析 / Parse CSV row
    [[nodiscard]] inline std::vector<std::string> parse_csv_row(
        const std::string& row, char delimiter = ',')
    {
        std::vector<std::string> fields;
        std::istringstream iss(row);
        std::string field;
        while (std::getline(iss, field, delimiter)) {
            fields.push_back(field);
        }
        return fields;
    }

    /// CSV 行生成 / Generate CSV row
    [[nodiscard]] inline std::string to_csv_row(
        const std::vector<std::string>& fields, char delimiter = ',')
    {
        if (fields.empty()) return {};
        std::string result = fields[0];
        for (std::size_t i = 1; i < fields.size(); ++i) {
            result += delimiter + fields[i];
        }
        return result;
    }

}  // namespace ospf::base
