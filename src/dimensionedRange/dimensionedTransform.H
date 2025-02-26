#pragma once

#include "dimensionedZipRange.H"
#include "dimensionedTransformRange.H"
#include "applyBinaryOp.H"

namespace FoamRanges {

namespace detail {

struct DimensionedTransform {

    template <class Rng, class UnaryOp>
    constexpr auto operator()(const Rng& rng, UnaryOp&& f) const {
        return makeDimensionedTransformRange
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

        return makeDimensionedTransformRange
        (
            rng1, rng2,
            ApplyBinaryOp<BinaryOp>(std::forward<BinaryOp>(f)),
            f(rng1.dimensions(), rng2.dimensions()),
            f(rng1.name(), rng2.name())
        );


    }
};

} // namespace detail

inline constexpr const auto dimensionedTransform =
    detail::DimensionedTransform{};

} // namespace FoamRanges