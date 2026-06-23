#pragma once

/// CSP1D 领域上下文 / CSP1D domain context

#include <ospf/framework/csp1d/domain.hpp>
#include <ospf/framework/csp1d/application.hpp>
#include <ospf/framework/context.hpp>

namespace ospf::framework::csp1d {

    /// CSP1D 上下文 / CSP1D context
    class Csp1dContext : public AbstractDomainContext {
    public:
        explicit Csp1dContext(ospf::core::MetaModel model = {})
            : model_(std::move(model)) {}

        [[nodiscard]] const std::string& domain_name() const override {
            static const std::string name = "CSP1D";
            return name;
        }

        [[nodiscard]] ospf::core::MetaModel& model() override { return model_; }
        [[nodiscard]] const ospf::core::MetaModel& model() const override { return model_; }

        /// 添加原料 / Add stock
        void add_stock(Stock stock) {
            stocks_.push_back(std::move(stock));
        }

        /// 添加需求件 / Add piece
        void add_piece(Piece piece) {
            pieces_.push_back(std::move(piece));
        }

        /// 获取原料 / Get stocks
        [[nodiscard]] const std::vector<Stock>& stocks() const noexcept { return stocks_; }

        /// 获取需求件 / Get pieces
        [[nodiscard]] const std::vector<Piece>& pieces() const noexcept { return pieces_; }

        /// 执行切割 / Execute cutting
        [[nodiscard]] ProductionPlan solve(CuttingAlgorithm& algorithm) {
            return algorithm.generate(stocks_, pieces_);
        }

    private:
        ospf::core::MetaModel model_;
        std::vector<Stock> stocks_;
        std::vector<Piece> pieces_;
    };

}  // namespace ospf::framework::csp1d
