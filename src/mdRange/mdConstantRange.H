#pragma once

#include "constantIterator.H"
#include "mdRange.H"

namespace FoamRanges {

// Note! This class does not allocate upon construction but simply stores the
// input value. This is done because we do not want this class to store array of
// 1D ranges because the number of 1D Ranges in a MdRange is generally not known
// compile time. Using this class as is requires special treatment in the
// operator[] of MdTransformZipRange.
template <typename Value> struct MdConstantRange : public MdRange {

    using iterator        = constantIterator<Value>;
    using value_type      = Value;
    using difference_type = std::ptrdiff_t;



    inline constexpr MdConstantRange(value_type c)
        : m_value(c) {}

    inline constexpr value_type get_value() const { return m_value; }

private:
    const value_type m_value;
};

template <typename Value> constexpr auto makeMdConstantRange(Value c) {
    return MdConstantRange<Value>(c);
}

} // namespace FoamRanges