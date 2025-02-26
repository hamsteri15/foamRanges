#pragma once

#include "geometricTransformRange.H"
#include "mdTransform.H"
#include "dimensionedTransform.H"
#include "applyBinaryOp.H"
namespace FoamRanges {

namespace detail {

struct GeometricTransform {

    template <class Rng, class UnaryOp>
    constexpr auto operator()(const Rng& rng, UnaryOp&& f) const {

        return make_geometric_transform_range
        (
            rng,
            std::forward<UnaryOp>(f),
            f(rng.dimensions()),
            f(rng.name())
        );
    }


    template <class Range1_t, class Range2_t, class BinaryOp>
    constexpr auto operator()(const Range1_t& rng1,
                              const Range2_t& rng2,
                              const BinaryOp& f) const {
        return make_geometric_transform_range
        (
            rng1,
            rng2,
            ApplyBinaryOp<BinaryOp>(f),
            f(rng1.dimensions(), rng2.dimensions()),
            f(rng1.name(), rng2.name())
        );


    }
};

} // namespace detail

inline constexpr const auto geometricTransform = detail::GeometricTransform{};

} // namespace FoamRanges