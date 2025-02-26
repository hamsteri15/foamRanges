#pragma once

#include "dimensionedTransformRange.H"
#include "dimensionedZipRange.H"
#include "geometricRange.H"
#include "mdTransformRange.H"



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
    using internal = decltype(makeDimensionedTransformRange(
        rng.internalField(), f, dimensions, name));

    return GeometricTransformRange<internal, boundary>{
        makeDimensionedTransformRange(
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
    using internal = decltype(makeDimensionedTransformRange(
        makeDimensionedZipRange(rng1.internalField(), rng2.internalField()),
        f,
        dimensions,
        name));

    return GeometricTransformRange<internal, boundary>(
        makeDimensionedTransformRange(
            makeDimensionedZipRange(rng1.internalField(),
                                       rng2.internalField()),
            f,
            dimensions,
            name),
        make_md_transform_range(rng1.boundaryField(), rng2.boundaryField(), f));
}

} // namespace FoamRanges
