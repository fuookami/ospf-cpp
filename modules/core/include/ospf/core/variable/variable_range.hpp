#pragma once
/// Variable range
namespace ospf::core {
    template<typename V = double>
    struct VariableRange {
        V lower;
        V upper;
    };
}