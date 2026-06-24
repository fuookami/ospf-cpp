#pragma once

/// 数据帧 / Data frame
/// 1:1 对应 Rust data_frame.rs

#include <string>
#include <vector>
#include <unordered_map>
#include <optional>
#include <algorithm>

namespace ospf::multiarray {

    /// 数据帧：列式数据结构 / Data frame: columnar data structure
    class DataFrame {
    public:
        DataFrame() = default;

        /// 添加列 / Add column
        void add_column(std::string name, std::vector<double> values) {
            columns_[std::move(name)] = std::move(values);
        }

        /// 获取列 / Get column
        [[nodiscard]] const std::vector<double>* column(const std::string& name) const {
            auto it = columns_.find(name);
            return it != columns_.end() ? &it->second : nullptr;
        }

        /// 获取可变列 / Get mutable column
        [[nodiscard]] std::vector<double>* column_mut(const std::string& name) {
            auto it = columns_.find(name);
            return it != columns_.end() ? &it->second : nullptr;
        }

        /// 列名列表 / Column names
        [[nodiscard]] std::vector<std::string> columns() const {
            std::vector<std::string> names;
            for (const auto& [k, _] : columns_) {
                names.push_back(k);
            }
            return names;
        }

        /// 行数 / Row count
        [[nodiscard]] std::size_t row_count() const {
            if (columns_.empty()) return 0;
            return columns_.begin()->second.size();
        }

        /// 列数 / Column count
        [[nodiscard]] std::size_t col_count() const { return columns_.size(); }

        /// 是否为空 / Is empty
        [[nodiscard]] bool empty() const { return columns_.empty(); }

        /// 获取单元格值 / Get cell value
        [[nodiscard]] std::optional<double> cell(const std::string& col_name, std::size_t row) const {
            auto* col = column(col_name);
            if (!col || row >= col->size()) return std::nullopt;
            return (*col)[row];
        }

    private:
        std::unordered_map<std::string, std::vector<double>> columns_;
    };

}  // namespace ospf::multiarray
