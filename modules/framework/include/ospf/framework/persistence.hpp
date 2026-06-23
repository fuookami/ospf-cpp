#pragma once

/// 持久化 / Persistence
/// 模型的保存和加载。

#include <ospf/core/model.hpp>
#include <nlohmann/json.hpp>
#include <fstream>
#include <string>

namespace ospf::framework {

    /// 持久化管理器 / Persistence manager
    class PersistenceManager {
    public:
        /// 保存模型到 JSON / Save model to JSON
        [[nodiscard]] static nlohmann::json to_json(const ospf::core::MetaModel& model) {
            nlohmann::json j;
            j["name"] = model.name();
            j["variable_count"] = model.variable_count();
            j["constraint_count"] = model.constraint_count();

            // 保存变量
            nlohmann::json vars = nlohmann::json::array();
            for (std::size_t i = 0; i < model.variable_count(); ++i) {
                const auto& var = model.variable(i);
                nlohmann::json v;
                v["name"] = var.name();
                v["lower"] = var.bounds().lower;
                v["upper"] = var.bounds().upper;
                vars.push_back(v);
            }
            j["variables"] = vars;

            // 保存约束名称
            nlohmann::json cons = nlohmann::json::array();
            for (const auto& c : model.constraints()) {
                cons.push_back(c.name);
            }
            j["constraints"] = cons;

            return j;
        }

        /// 从 JSON 加载模型（简化版） / Load model from JSON (simplified)
        [[nodiscard]] static ospf::core::MetaModel from_json(const nlohmann::json& j) {
            ospf::core::MetaModel model(j["name"].get<std::string>());
            return model;
        }

        /// 保存到文件 / Save to file
        static bool save_to_file(const ospf::core::MetaModel& model, const std::string& path) {
            auto j = to_json(model);
            std::ofstream file(path);
            if (!file.is_open()) return false;
            file << j.dump(2);
            return true;
        }

        /// 从文件加载 / Load from file
        [[nodiscard]] static std::optional<ospf::core::MetaModel> load_from_file(const std::string& path) {
            std::ifstream file(path);
            if (!file.is_open()) return std::nullopt;
            nlohmann::json j;
            file >> j;
            return from_json(j);
        }
    };

}  // namespace ospf::framework
