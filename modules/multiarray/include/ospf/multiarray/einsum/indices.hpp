#pragma once

/// Einsum 索引 / Einsum indices
/// 1:1 对应 Rust einsum/indices.rs

#include <algorithm>
#include <string>
#include <vector>
#include <unordered_map>

namespace ospf::multiarray::einsum {

    /// Einsum 索引解析结果 / Einsum index parsing result
    struct EinsumIndices {
        /// 输入索引标签 / Input index labels
        std::vector<std::vector<char>> input_labels;
        /// 输出索引标签 / Output index labels
        std::vector<char> output_labels;
        /// 所有唯一标签 / All unique labels
        std::vector<char> all_labels;

        /// 解析 einsum 方程 / Parse einsum equation
        /// 格式: "ij,jk->ik" (输入用逗号分隔，-> 后为输出)
        [[nodiscard]] static EinsumIndices parse(const std::string& equation) {
            EinsumIndices result;
            auto arrow = equation.find("->");
            std::string inputs_part = (arrow != std::string::npos)
                ? equation.substr(0, arrow) : equation;
            std::string output_part = (arrow != std::string::npos)
                ? equation.substr(arrow + 2) : "";

            // 解析输入标签 / Parse input labels
            std::vector<char> current;
            for (char c : inputs_part) {
                if (c == ',') {
                    result.input_labels.push_back(std::move(current));
                    current.clear();
                } else if (c != ' ') {
                    current.push_back(c);
                    result.all_labels.push_back(c);
                }
            }
            if (!current.empty()) {
                result.input_labels.push_back(std::move(current));
            }

            // 解析输出标签 / Parse output labels
            for (char c : output_part) {
                if (c != ' ') {
                    result.output_labels.push_back(c);
                }
            }

            // 去重 / Deduplicate all_labels
            std::sort(result.all_labels.begin(), result.all_labels.end());
            result.all_labels.erase(
                std::unique(result.all_labels.begin(), result.all_labels.end()),
                result.all_labels.end()
            );

            return result;
        }
    };

}  // namespace ospf::multiarray::einsum
