#pragma once
/// 复杂块生成器 / Complex block generator
/// 1:1 对应 Rust bpp3d/domain/block_loading/service/complex_block_generator.rs
/// 生成多物品组合的复杂块

#include <cstddef>
#include <vector>
#include <optional>

namespace ospf::framework::bpp3d {

    /// 复杂块生成器配置 / Complex block generator configuration
    /// 对应 Rust ComplexBlockGeneratorConfig
    struct ComplexBlockGeneratorConfig {
        /// 最大组合大小 / Maximum combination size
        std::size_t max_combination_size = 3;
        /// 启用旋转 / Enable rotation
        bool with_rotation = true;
    };

    /// 复杂块生成器 / Complex block generator
    /// 对应 Rust ComplexBlockGenerator
    /// 生成多物品组合的复杂块
    class ComplexBlockGenerator {
    public:
        explicit ComplexBlockGenerator(ComplexBlockGeneratorConfig config = {})
            : config_(std::move(config)) {}

        /// 配置 / Configuration
        [[nodiscard]] const ComplexBlockGeneratorConfig& config() const noexcept { return config_; }

        /// 最大组合大小 / Max combination size
        [[nodiscard]] std::size_t max_combination_size() const noexcept { return config_.max_combination_size; }

    private:
        ComplexBlockGeneratorConfig config_;
    };

}  // namespace ospf::framework::bpp3d
