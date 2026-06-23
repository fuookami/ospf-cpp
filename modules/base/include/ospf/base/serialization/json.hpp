#pragma once

/// JSON 序列化 / JSON serialization
/// 使用 nlohmann-json 进行 JSON 序列化/反序列化。

#include <nlohmann/json.hpp>
#include <string>

namespace ospf::base {

    using Json = nlohmann::json;

    /// 序列化为 JSON 字符串 / Serialize to JSON string
    template<typename T>
    [[nodiscard]] std::string to_json(const T& value) {
        return Json(value).dump();
    }

    /// 从 JSON 字符串反序列化 / Deserialize from JSON string
    template<typename T>
    [[nodiscard]] T from_json(const std::string& json_str) {
        return Json::parse(json_str).get<T>();
    }

}  // namespace ospf::base
