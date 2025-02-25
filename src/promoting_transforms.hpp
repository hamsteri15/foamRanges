#pragma once

#include "traits.hpp"

#include "constant_range.hpp"
#include "transform.hpp"

#include "dimensioned_constant_range.hpp"
#include "dimensioned_transform.hpp"

#include "geometric_constant_range.hpp"
#include "geometric_transform.hpp"

#include "md_constant_range.hpp"
#include "md_transform.hpp"

#include "ensure_dimensioned.hpp"

namespace Foam {

namespace detail {

template <class T1, class T2, class BinaryOp>
constexpr auto
rng_promoting_transform(const T1& lhs, const T2& rhs, BinaryOp f) {

    if constexpr (BothFoamField_v<T1, T2>) {
        return FoamRanges::transform(lhs, rhs, f);
    } else if constexpr (IsFoamField_v<T1>) {
        return FoamRanges::transform(
            lhs, FoamRanges::make_constant_range(rhs, std::size(lhs)), f);
    } else {
        return FoamRanges::transform(
            FoamRanges::make_constant_range(lhs, std::size(rhs)), rhs, f);
    }
}

template <class T1, class T2, class BinaryOp>
constexpr auto
dimrng_promoting_transform(const T1& lhs, const T2& rhs, const BinaryOp& f) {

    using namespace FoamRanges;

    if constexpr (BothDimensionedField_v<T1, T2>) {

        return dimensioned_transform(lhs, rhs, f);
    } else if constexpr (IsFoamDimensionedField_v<T1>) {
        return dimensioned_transform(
            lhs,
            make_dimensioned_constant_range(
                EnsureDimensioned<T2>(rhs).type, lhs.size(), get_mesh(lhs)),
            f);
    } else {
        return dimensioned_transform(
            make_dimensioned_constant_range(
                EnsureDimensioned<T1>(lhs).type, rhs.size(), get_mesh(rhs)),
            rhs,
            f);
    }
}


template <class T1, class T2, class BinaryOp>
constexpr auto md_promoting_transform(T1&& lhs, T2&& rhs, BinaryOp&& f) {

    using namespace FoamRanges;

    if constexpr (BothFieldField_v<std::decay_t<T1>, std::decay_t<T2>>) {
        return md_transform(std::forward<T1>(lhs), std::forward<T2>(rhs), std::forward<BinaryOp>(f));
    } else if constexpr (IsFoamFieldField_v<std::decay_t<T1>>) {
        return md_transform(
            std::forward<T1>(lhs), make_md_constant_range(rhs), std::forward<BinaryOp>(f));
    } else {
        return md_transform(
            make_md_constant_range(lhs), std::forward<T2>(rhs), std::forward<BinaryOp>(f));
    }

}


template <class T1, class T2, class BinaryOp>
constexpr auto
georng_promoting_transform(const T1& lhs, const T2& rhs, const BinaryOp& f) {

    using namespace FoamRanges;

    if constexpr (BothGeometricField_v<T1, T2>) {
        return geometric_transform(lhs, rhs, f);
    } else if constexpr (IsFoamGeometricField_v<T1>) {
        return geometric_transform(
            lhs,
            make_geometric_constant_range(EnsureDimensioned<T2>(rhs).type,
                                          get_mesh(lhs)),
            f);
    } else {
        return geometric_transform(
            make_geometric_constant_range(EnsureDimensioned<T1>(lhs).type,
                                          get_mesh(rhs)),
            rhs,
            f);
    }
}

} // namespace detail

} // namespace Foam