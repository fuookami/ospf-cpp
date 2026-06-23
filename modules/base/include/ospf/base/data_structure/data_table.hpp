#pragma once

/// 数据表 / Data table
/// 类似 DataFrame 的表格数据结构。

#include <cstddef>
#include <string>
#include <unordered_map>
#include <vector>

namespace ospf::base {

    /// 数据表：按列名存储数据 / Data table: stores data by column name
    class DataTable {
    public:
        using Row = std::unordered_map<std::string, std::string>;

        void add_column(const std::string& name) {
            if (column_index_.count(name) == 0) {
                column_index_[name] = columns_.size();
                columns_.emplace_back(name, std::vector<std::string>{});
            }
        }

        void add_row(const Row& row) {
            // 确保所有列存在 / Ensure all columns exist
            for (const auto& [key, _] : row) {
                add_column(key);
            }
            // 填充行数据 / Fill row data
            for (auto& [name, col] : columns_) {
                auto it = row.find(name);
                if (it != row.end()) {
                    col.push_back(it->second);
                } else {
                    col.emplace_back();  // 空值 / Empty
                }
            }
        }

        [[nodiscard]] const std::string& cell(std::size_t row, const std::string& col) const {
            auto it = column_index_.find(col);
            return columns_[it->second].second[row];
        }

        [[nodiscard]] const std::vector<std::string>& column(const std::string& name) const {
            auto it = column_index_.find(name);
            return columns_[it->second].second;
        }

        [[nodiscard]] std::size_t row_count() const {
            if (columns_.empty()) return 0;
            return columns_[0].second.size();
        }

        [[nodiscard]] std::size_t col_count() const { return columns_.size(); }

        [[nodiscard]] std::vector<std::string> column_names() const {
            std::vector<std::string> names;
            for (const auto& [name, _] : columns_) {
                names.push_back(name);
            }
            return names;
        }

    private:
        std::vector<std::pair<std::string, std::vector<std::string>>> columns_;
        std::unordered_map<std::string, std::size_t> column_index_;
    };

}  // namespace ospf::base
