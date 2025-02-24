#pragma once

#include "range.hpp"
#include "transform_iterator.hpp"

namespace FoamRanges {

template <typename UnaryFunction, typename Iterator>
struct TransformRange
    : public Range<detail::transform_iterator<UnaryFunction, Iterator>> {

    using parent = Range<detail::transform_iterator<UnaryFunction, Iterator>>;

    inline constexpr TransformRange() = default;

    inline constexpr TransformRange(Iterator             first,
                                    Iterator             last,
                                    const UnaryFunction& f)
        : parent(detail::make_transform_iterator(first, f),
                 detail::make_transform_iterator(last, f)) {}
};

template <typename Function, typename Iterator>
inline constexpr auto
make_transform_range(Iterator first, Iterator last, const Function& f) {
    return TransformRange<Function, Iterator>(first, last, f);
}

template <typename Function, class Range_t>
inline constexpr auto make_transform_range(const Range_t&  rng,
                                           const Function& f) {
    return make_transform_range(std::begin(rng), std::end(rng), f);
}

} // namespace FoamRanges