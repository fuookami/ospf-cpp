#pragma once

/// 领域上下文 / Domain context
/// AbstractDomainContext 是领域框架的基础接口。

#include <ospf/core/model.hpp>
#include <string>
#include <unordered_map>

namespace ospf::framework {

    /// 领域上下文基类 / Domain context base class
    class AbstractDomainContext {
    public:
        virtual ~AbstractDomainContext() = default;

        /// 领域名称 / Domain name
        [[nodiscard]] virtual const std::string& domain_name() const = 0;

        /// 获取关联的模型 / Get associated model
        [[nodiscard]] virtual ospf::core::MetaModel& model() = 0;
        [[nodiscard]] virtual const ospf::core::MetaModel& model() const = 0;

        /// 初始化上下文 / Initialize context
        virtual void initialize() {}

        /// 清理上下文 / Cleanup context
        virtual void cleanup() {}
    };

    /// 通用领域上下文 / Generic domain context
    class DomainContext : public AbstractDomainContext {
    public:
        explicit DomainContext(std::string name, ospf::core::MetaModel model)
            : name_(std::move(name)), model_(std::move(model)) {}

        [[nodiscard]] const std::string& domain_name() const override { return name_; }
        [[nodiscard]] ospf::core::MetaModel& model() override { return model_; }
        [[nodiscard]] const ospf::core::MetaModel& model() const override { return model_; }

    private:
        std::string name_;
        ospf::core::MetaModel model_;
    };

}  // namespace ospf::framework
