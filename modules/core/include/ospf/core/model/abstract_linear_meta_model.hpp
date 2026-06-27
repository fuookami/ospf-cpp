#pragma once
/// Abstract linear meta model
#include <ospf/core/model.hpp>
namespace ospf::core {
    class AbstractLinearMetaModel : public MetaModel {
    public:
        using MetaModel::MetaModel;
        void add_linear_constraint(std::string n, ospf::math::symbol::Inequality i) { add_constraint(std::move(n), std::move(i)); }
        void set_linear_objective(std::string n, ObjectiveSense s, ospf::math::symbol::Expression e) { set_objective(std::move(n), s, std::move(e)); }
    };
}