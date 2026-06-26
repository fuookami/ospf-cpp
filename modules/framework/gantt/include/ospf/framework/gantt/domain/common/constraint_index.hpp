#pragma once
/// Gantt domain common/constraint_index /// 1:1 对应 Rust gantt/domain/common/constraint_index.rs
/// ConstraintIndexKey + ConstraintIndexEntry + ConstraintIndexMap

#include <cstddef>
#include <optional>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

namespace ospf::framework::gantt {

    // ============================================================================
    // 约束索引键 / Constraint index key
    // 对应 Rust ConstraintIndexKey
    // ============================================================================

    /// 约束索引键 / Constraint index key
    /// 对应 Rust ConstraintIndexKey (enum: TaskCompilation/ExecutorCompilation/Bunch/Custom)
    using ConstraintIndexKey = std::variant<
        struct CKTaskCompilation,
        struct CKExecutorCompilation,
        struct CKBunch,
        struct CKCustom
    >;

    struct CKTaskCompilation {
        std::size_t task_index = 0;
        bool operator==(const CKTaskCompilation&) const noexcept = default;
    };
    struct CKExecutorCompilation {
        std::string executor_id;
        bool operator==(const CKExecutorCompilation& o) const noexcept { return executor_id == o.executor_id; }
    };
    struct CKBunch {
        std::size_t bunch_index = 0;
        bool operator==(const CKBunch&) const noexcept = default;
    };
    struct CKCustom {
        std::string family;
        std::string key;
        bool operator==(const CKCustom& o) const noexcept { return family == o.family && key == o.key; }
    };

    /// 创建任务编译 key / Create task-compilation key
    [[nodiscard]] inline ConstraintIndexKey task_compilation_key(std::size_t task_index) {
        return CKTaskCompilation{task_index};
    }

    /// 创建执行器编译 key / Create executor-compilation key
    [[nodiscard]] inline ConstraintIndexKey executor_compilation_key(std::string executor_id) {
        return CKExecutorCompilation{std::move(executor_id)};
    }

    /// 创建自定义 key / Create custom key
    [[nodiscard]] inline ConstraintIndexKey custom_key(std::string family, std::string key) {
        return CKCustom{std::move(family), std::move(key)};
    }

    // ============================================================================
    // 约束索引条目 / Constraint index entry
    // 对应 Rust ConstraintIndexEntry
    // ============================================================================

    /// 约束索引条目 / Constraint index entry
    struct ConstraintIndexEntry {
        ConstraintIndexKey key;
        std::string constraint_name;
        std::size_t dual_index = 0;
    };

    // ============================================================================
    // 约束索引映射 / Constraint index map
    // 对应 Rust ConstraintIndexMap
    // ============================================================================

    /// 约束索引映射 / Constraint index map
    struct ConstraintIndexMap {
        std::vector<ConstraintIndexEntry> entries;
        std::unordered_map<std::string, std::size_t> name_to_entry;

        /// 创建 / Create
        [[nodiscard]] static ConstraintIndexMap create() { return ConstraintIndexMap{}; }

        /// 条目数量 / Entry count
        [[nodiscard]] std::size_t size() const noexcept { return entries.size(); }

        /// 注册 / Register
        void register_key(ConstraintIndexKey key, std::string name, std::size_t dual_idx) {
            ConstraintIndexEntry entry;
            entry.key = std::move(key);
            entry.constraint_name = name;
            entry.dual_index = dual_idx;
            name_to_entry[name] = entries.size();
            entries.push_back(std::move(entry));
        }

        /// 根据 key 获取 / Get by key
        [[nodiscard]] const ConstraintIndexEntry* get(const ConstraintIndexKey& key) const {
            for (const auto& entry : entries) {
                if (entry.key == key) return &entry;
            }
            return nullptr;
        }

        /// 根据名称获取 / Get by name
        [[nodiscard]] const ConstraintIndexEntry* get_by_name(const std::string& name) const {
            auto it = name_to_entry.find(name);
            if (it != name_to_entry.end()) return &entries[it->second];
            return nullptr;
        }

        /// 获取对偶值 / Get dual value
        [[nodiscard]] std::optional<double> dual_value(
            const ConstraintIndexKey& key,
            const std::vector<double>& duals) const
        {
            const auto* entry = get(key);
            if (!entry || entry->dual_index >= duals.size()) return std::nullopt;
            return duals[entry->dual_index];
        }

        /// 注册任务编译约束 / Register task compilation constraints
        void register_task_compilation_constraints(
            std::size_t task_count,
            const std::unordered_map<std::string, std::size_t>& name_to_idx)
        {
            for (std::size_t i = 0; i < task_count; ++i) {
                std::string name = "task_compilation_" + std::to_string(i);
                auto it = name_to_idx.find(name);
                if (it != name_to_idx.end()) {
                    register_key(task_compilation_key(i), name, it->second);
                }
            }
        }
    };

}  // namespace ospf::framework::gantt
