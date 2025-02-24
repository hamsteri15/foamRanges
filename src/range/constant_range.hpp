#pragma once

#include "constant_iterator.hpp"
#include "range.hpp"

namespace FoamRanges{

template <typename Value>
struct ConstantRange : public Range<constant_iterator<Value>> {

private:
    using parent          = Range<constant_iterator<Value>>;
    using value_type      = typename parent::value_type;
    using difference_type = typename parent::difference_type;

public:

    inline ConstantRange() = default;

    inline constexpr ConstantRange(value_type c, difference_type n)
        : parent(constant_iterator<Value>(c, difference_type(0)),
                 constant_iterator<Value>(c, n)) {}
};

template <typename Value, typename Size>
inline constexpr auto make_constant_range(Value c, Size n) {

    return ConstantRange<Value>(c, n);
}

} // namespace FoamRanges