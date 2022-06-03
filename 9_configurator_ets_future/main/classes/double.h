#pragma once
#define PS2_EPSILON_MM 1.0e-3

#define PS2_EPSILON std::numeric_limits<double>::epsilon()

/** namespace gt */
namespace gt {

constexpr inline bool isZero(double a, double aEpsilon = PS2_EPSILON) noexcept {
    return (-aEpsilon < a && a < aEpsilon);
}
constexpr inline bool isEqual(double firstValue, double secondValue
                       , const double aEpsilon = PS2_EPSILON) noexcept {
    return isZero(firstValue - secondValue, aEpsilon);
}
constexpr inline bool isGreater(double firstValue, double secondValue
                     , const double aEpsilon = PS2_EPSILON) noexcept {
    return firstValue > secondValue + aEpsilon;
}
constexpr inline bool isLess(double firstValue, double secondValue
                          , const double aEpsilon = PS2_EPSILON) noexcept {
    return firstValue < secondValue - aEpsilon;
}

} // end namespace gt
