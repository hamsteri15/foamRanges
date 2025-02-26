#pragma once

#include "Range.H"
#include "transformIterator.H"

namespace FoamRanges {

template <typename UnaryFunction, typename Iterator>
struct TransformRange
    : public Range<detail::transformIterator<UnaryFunction, Iterator>> {

    using parent = Range<detail::transformIterator<UnaryFunction, Iterator>>;

    inline constexpr TransformRange() = default;

    inline constexpr TransformRange(Iterator             first,
                                    Iterator             last,
                                    const UnaryFunction& f)
        : parent(detail::makeTransformIterator(first, f),
                 detail::makeTransformIterator(last, f)) {}
};

template <typename Function, typename Iterator>
inline constexpr auto
makeTransformRange(Iterator first, Iterator last, const Function& f) {
    return TransformRange<Function, Iterator>(first, last, f);
}

template <typename Function, class Range_t>
inline constexpr auto makeTransformRange(const Range_t&  rng,
                                           const Function& f) {
    return makeTransformRange(std::begin(rng), std::end(rng), f);
}

} // namespace FoamRanges