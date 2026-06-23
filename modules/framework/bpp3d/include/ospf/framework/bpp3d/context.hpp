#pragma once

/// BPP3D 领域上下文 / BPP3D domain context

#include <ospf/framework/bpp3d/domain.hpp>
#include <ospf/framework/bpp3d/application.hpp>
#include <ospf/framework/context.hpp>
#include <ospf/core/solver.hpp>

namespace ospf::framework::bpp3d {

    /// BPP3D 上下文 / BPP3D context
    class Bpp3dContext : public AbstractDomainContext {
    public:
        explicit Bpp3dContext(ospf::core::MetaModel model = {})
            : model_(std::move(model)) {}

        [[nodiscard]] const std::string& domain_name() const override {
            static const std::string name = "BPP3D";
            return name;
        }

        [[nodiscard]] ospf::core::MetaModel& model() override { return model_; }
        [[nodiscard]] const ospf::core::MetaModel& model() const override { return model_; }

        /// 添加物品 / Add items
        void add_item(Item item) {
            items_.push_back(std::move(item));
        }

        /// 设置容器 / Set bin
        void set_bin(Bin bin) {
            bin_ = std::move(bin);
        }

        /// 获取物品 / Get items
        [[nodiscard]] const std::vector<Item>& items() const noexcept { return items_; }

        /// 获取容器 / Get bin
        [[nodiscard]] const std::optional<Bin>& bin() const noexcept { return bin_; }

        /// 执行打包 / Execute packing
        [[nodiscard]] PackingSolution solve(PackingAlgorithm& algorithm) {
            if (!bin_.has_value()) {
                return PackingSolution{};
            }
            return algorithm.pack(bin_.value(), items_);
        }

    private:
        ospf::core::MetaModel model_;
        std::vector<Item> items_;
        std::optional<Bin> bin_;
    };

}  // namespace ospf::framework::bpp3d
