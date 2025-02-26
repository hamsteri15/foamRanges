#pragma once

#include "zipRange.H"
#include "transformRange.H"
#include "applyBinaryOp.H"

namespace FoamRanges {

namespace detail {

struct Transform {


    template <class Range_t, class UnaryOp>
    inline constexpr auto operator()(const Range_t& rng, const UnaryOp& f) const {
        return makeTransformRange(rng, f);
    }



    template <class Range1_t, class Range2_t, class BinaryOp>
    inline constexpr auto
    operator()(const Range1_t& rng1, const Range2_t& rng2, const BinaryOp& f) const {
        return makeTransformRange(makeZipRange(rng1, rng2),
                                    ApplyBinaryOp<BinaryOp>(f));
    }
};

} // namespace detail

//This is the niebloid we expose in FoamRanges to allow for customization of makeTransformRange and zip
//inline constexpr detail::Transform transform{};
inline constexpr const auto transform = detail::Transform{};


} // namespace FoamRanges