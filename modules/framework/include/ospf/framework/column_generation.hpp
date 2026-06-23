#pragma once

/// 列生成 / Column generation
/// 实现列生成生命周期：register_model / add_columns / remove_columns / refresh_shadow_price / finalize。

#include <ospf/core/solver.hpp>
#include <functional>
#include <string>
#include <vector>

namespace ospf::framework {

    /// 列生成阶段 / Column generation stage
    enum class ColumnGenerationStage : std::uint8_t {
        RegisterModel,        ///< 注册模型
        AddColumns,           ///< 添加列
        RemoveColumns,        ///< 移除列
        RefreshShadowPrice,   ///< 刷新影子价格
        Finalize,             ///< 完成
    };

    /// 影子价格 / Shadow price
    struct ShadowPrice {
        std::string constraint_name;
        double price = 0.0;
    };

    /// 列信息 / Column info
    struct ColumnInfo {
        std::string name;
        double cost = 0.0;
        std::vector<double> coefficients;
        bool active = true;
    };

    /// 列生成求解器 / Column generation solver
    class ColumnGenerationSolver {
    public:
        using StageCallback = std::function<void(ColumnGenerationStage)>;

        /// 设置阶段回调 / Set stage callback
        void on_stage(StageCallback callback) {
            callback_ = std::move(callback);
        }

        /// 注册模型 / Register model
        void register_model(ospf::core::MetaModel& model) {
            model_ = &model;
            notify(ColumnGenerationStage::RegisterModel);
        }

        /// 添加列 / Add columns
        void add_columns(std::vector<ColumnInfo> columns) {
            for (auto& col : columns) {
                columns_.push_back(std::move(col));
            }
            notify(ColumnGenerationStage::AddColumns);
        }

        /// 移除列 / Remove columns
        void remove_columns(const std::vector<std::string>& names) {
            for (const auto& name : names) {
                std::erase_if(columns_, [&](const ColumnInfo& c) {
                    return c.name == name;
                });
            }
            notify(ColumnGenerationStage::RemoveColumns);
        }

        /// 刷新影子价格 / Refresh shadow prices
        void refresh_shadow_prices(std::vector<ShadowPrice> prices) {
            shadow_prices_ = std::move(prices);
            notify(ColumnGenerationStage::RefreshShadowPrice);
        }

        /// 完成 / Finalize
        void finalize() {
            notify(ColumnGenerationStage::Finalize);
        }

        /// 获取列 / Get columns
        [[nodiscard]] const std::vector<ColumnInfo>& columns() const noexcept { return columns_; }

        /// 获取影子价格 / Get shadow prices
        [[nodiscard]] const std::vector<ShadowPrice>& shadow_prices() const noexcept {
            return shadow_prices_;
        }

        /// 活跃列数量 / Active column count
        [[nodiscard]] std::size_t active_column_count() const {
            std::size_t count = 0;
            for (const auto& col : columns_) {
                if (col.active) ++count;
            }
            return count;
        }

    private:
        void notify(ColumnGenerationStage stage) {
            if (callback_) callback_(stage);
        }

        ospf::core::MetaModel* model_ = nullptr;
        std::vector<ColumnInfo> columns_;
        std::vector<ShadowPrice> shadow_prices_;
        StageCallback callback_;
    };

}  // namespace ospf::framework
