#pragma once

/// 单位系统 / Unit system
/// 定义单位及其换算。

#include <ospf/quantities/dimension.hpp>
#include <string>

namespace ospf::quantities {

    /// 单位 / Unit
    struct Unit {
        Dimension dimension;     ///< 维度 / Dimension
        double    to_si_factor;  ///< 转换到 SI 单位的系数 / Factor to convert to SI
        std::string name;        ///< 单位名称 / Unit name
        std::string symbol;      ///< 单位符号 / Unit symbol

        [[nodiscard]] constexpr bool operator==(const Unit&) const = default;
    };

    // -- SI 基本单位 / SI base units ------------------------------------------

    inline const Unit UNIT_METER      = {DIM_LENGTH,      1.0, "meter",      "m"};
    inline const Unit UNIT_KILOGRAM   = {DIM_MASS,        1.0, "kilogram",   "kg"};
    inline const Unit UNIT_SECOND     = {DIM_TIME,        1.0, "second",     "s"};
    inline const Unit UNIT_AMPERE     = {DIM_CURRENT,     1.0, "ampere",     "A"};
    inline const Unit UNIT_KELVIN     = {DIM_TEMPERATURE, 1.0, "kelvin",     "K"};
    inline const Unit UNIT_MOLE       = {DIM_AMOUNT,      1.0, "mole",       "mol"};
    inline const Unit UNIT_CANDELA    = {DIM_LUMINOSITY,  1.0, "candela",    "cd"};

    // -- SI 派生单位 / SI derived units ---------------------------------------

    inline const Unit UNIT_NEWTON     = {DIM_FORCE,   1.0, "newton",    "N"};
    inline const Unit UNIT_JOULE      = {DIM_ENERGY,  1.0, "joule",     "J"};
    inline const Unit UNIT_WATT       = {DIM_POWER,   1.0, "watt",      "W"};
    inline const Unit UNIT_SQUARE_METER = {DIM_AREA,  1.0, "square meter", "m²"};
    inline const Unit UNIT_CUBIC_METER  = {DIM_VOLUME, 1.0, "cubic meter", "m³"};

    // -- 非 SI 单位 / Non-SI units --------------------------------------------

    inline const Unit UNIT_KILOMETER  = {DIM_LENGTH,  1000.0, "kilometer",  "km"};
    inline const Unit UNIT_CENTIMETER = {DIM_LENGTH,  0.01,   "centimeter", "cm"};
    inline const Unit UNIT_MILLIMETER = {DIM_LENGTH,  0.001,  "millimeter", "mm"};
    inline const Unit UNIT_GRAM       = {DIM_MASS,    0.001,  "gram",       "g"};
    inline const Unit UNIT_TONNE      = {DIM_MASS,    1000.0, "tonne",      "t"};
    inline const Unit UNIT_HOUR       = {DIM_TIME,    3600.0, "hour",       "h"};
    inline const Unit UNIT_MINUTE     = {DIM_TIME,    60.0,   "minute",     "min"};

    /// 单位换算 / Unit conversion
    [[nodiscard]] inline double convert(double value, const Unit& from, const Unit& to) {
        // 先转为 SI，再转为目标单位
        double si_value = value * from.to_si_factor;
        return si_value / to.to_si_factor;
    }

    /// UnitTrait：单位必须满足的概念 / UnitTrait: concept that units must satisfy
    template<typename U>
    concept UnitTrait = requires {
        { U::dimension } -> std::convertible_to<Dimension>;
        { U::name } -> std::convertible_to<std::string>;
    };

}  // namespace ospf::quantities
