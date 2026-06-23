#pragma once

/// Gantt 排程领域上下文 / Gantt scheduling domain context

#include <ospf/framework/gantt/domain.hpp>
#include <ospf/framework/gantt/application.hpp>
#include <ospf/framework/context.hpp>

namespace ospf::framework::gantt {

    /// Gantt 上下文 / Gantt context
    class GanttContext : public AbstractDomainContext {
    public:
        explicit GanttContext(ospf::core::MetaModel model = {})
            : model_(std::move(model)) {}

        [[nodiscard]] const std::string& domain_name() const override {
            static const std::string name = "Gantt";
            return name;
        }

        [[nodiscard]] ospf::core::MetaModel& model() override { return model_; }
        [[nodiscard]] const ospf::core::MetaModel& model() const override { return model_; }

        /// 添加任务 / Add task
        void add_task(Task task) {
            tasks_.push_back(std::move(task));
        }

        /// 添加资源 / Add resource
        void add_resource(Resource resource) {
            resources_.push_back(std::move(resource));
        }

        /// 获取任务 / Get tasks
        [[nodiscard]] const std::vector<Task>& tasks() const noexcept { return tasks_; }

        /// 获取资源 / Get resources
        [[nodiscard]] const std::vector<Resource>& resources() const noexcept { return resources_; }

        /// 执行排程 / Execute scheduling
        [[nodiscard]] ScheduleResult solve(SchedulingAlgorithm& algorithm) {
            return algorithm.schedule(tasks_, resources_);
        }

    private:
        ospf::core::MetaModel model_;
        std::vector<Task> tasks_;
        std::vector<Resource> resources_;
    };

}  // namespace ospf::framework::gantt
