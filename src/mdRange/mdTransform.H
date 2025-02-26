#pragma once

#include "mdTransformRange.H"

namespace FoamRanges {
namespace detail {

struct MdTransform {

    template <class Rng, class UnaryOp>
    constexpr auto operator()(Rng&& rng, UnaryOp&& f) const {
        return make_md_transform_range(std::forward<Rng>(rng),
                                       std::forward<UnaryOp>(f));
    }

    template <class Rng1, class Rng2, class BinaryOp>
    constexpr auto operator()(Rng1&& rng1, Rng2&& rng2, BinaryOp&& f) const {
        return make_md_transform_range(
            std::forward<Rng1>(rng1),
            std::forward<Rng2>(rng2),
            ApplyBinaryOp<BinaryOp>(std::forward<BinaryOp>(f)));
    }
};

} // namespace detail

inline constexpr const auto mdTransform = detail::MdTransform{};

} // namespace FoamRanges