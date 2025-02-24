#pragma once

#include "dimensioned_zip_range.hpp"
#include "dimensioned_transform_range.hpp"
#include "apply_binary_op.hpp"

namespace FoamRanges {

namespace detail {

struct DimensionedTransform {

    template <class Rng, class UnaryOp>
    constexpr auto operator()(const Rng& rng, UnaryOp&& f) const {
        return make_dimensioned_transform_range
        (
            rng,
            std::forward<UnaryOp>(f),
            f(rng.dimensions()),
            f(rng.name())
        );
    }

    template <class Rng1, class Rng2, class BinaryOp>
    constexpr auto
    operator()(const Rng1& rng1, const Rng2& rng2, BinaryOp&& f) const {

        return make_dimensioned_transform_range
        (
            rng1, rng2,
            ApplyBinaryOp<BinaryOp>(std::forward<BinaryOp>(f)),
            f(rng1.dimensions(), rng2.dimensions()),
            f(rng1.name(), rng2.name())
        );


    }
};

} // namespace detail

inline constexpr const auto dimensioned_transform =
    detail::DimensionedTransform{};

} // namespace FoamRanges