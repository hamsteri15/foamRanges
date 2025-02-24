#pragma once

#include "geometric_transform_range.hpp"
#include "md_transform.hpp"
#include "dimensioned_transform.hpp"
#include "apply_binary_op.hpp"
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

inline constexpr const auto geometric_transform = detail::GeometricTransform{};

} // namespace FoamRanges