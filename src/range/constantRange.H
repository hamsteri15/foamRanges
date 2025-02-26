#pragma once

#include "constantIterator.H"
#include "Range.H"

namespace FoamRanges{

template <typename Value>
struct ConstantRange : public Range<constantIterator<Value>> {

private:
    using parent          = Range<constantIterator<Value>>;
    using value_type      = typename parent::value_type;
    using difference_type = typename parent::difference_type;

public:

    inline ConstantRange() = default;

    inline constexpr ConstantRange(value_type c, difference_type n)
        : parent(constantIterator<Value>(c, difference_type(0)),
                 constantIterator<Value>(c, n)) {}
};

template <typename Value, typename Size>
inline constexpr auto makeConstantRange(Value c, Size n) {

    return ConstantRange<Value>(c, n);
}

} // namespace FoamRanges