#pragma once

#include "dimensioned_transform_range.hpp"
#include "dimensioned_zip_range.hpp"
#include "geometric_range.hpp"
#include "md_transform_range.hpp"



namespace FoamRanges {


template <class Internal, class Boundary>
struct GeometricTransformRange : public GeometricRange<Internal, Boundary> {

    constexpr GeometricTransformRange(Internal&& internal, Boundary&& boundary)
        : GeometricRange<Internal, Boundary>(std::forward<Internal>(internal),
                                              std::forward<Boundary>(boundary)) {}
};



template <typename Function, class GeomRange_t>
constexpr auto
make_geometric_transform_range(const GeomRange_t&        rng,
                               Function                  f,
                               const Foam::dimensionSet& dimensions,
                               const Foam::word&         name) {


    using boundary = decltype(make_md_transform_range(rng.boundaryField(), f));
    using internal = decltype(make_dimensioned_transform_range(
        rng.internalField(), f, dimensions, name));

    return GeometricTransformRange<internal, boundary>{
        make_dimensioned_transform_range(
            rng.internalField(), f, dimensions, name),
        make_md_transform_range(rng.boundaryField(), f)};

}

template <typename Function, class Rng1, class Rng2>
constexpr auto
make_geometric_transform_range(const Rng1&               rng1,
                               const Rng2&               rng2,
                               const Function&           f,
                               const Foam::dimensionSet& dimensions,
                               const Foam::word&         name) {

    using boundary = decltype(make_md_transform_range(
        rng1.boundaryField(), rng2.boundaryField(), f));
    using internal = decltype(make_dimensioned_transform_range(
        make_dimensioned_zip_range(rng1.internalField(), rng2.internalField()),
        f,
        dimensions,
        name));

    return GeometricTransformRange<internal, boundary>(
        make_dimensioned_transform_range(
            make_dimensioned_zip_range(rng1.internalField(),
                                       rng2.internalField()),
            f,
            dimensions,
            name),
        make_md_transform_range(rng1.boundaryField(), rng2.boundaryField(), f));
}

} // namespace FoamRanges
