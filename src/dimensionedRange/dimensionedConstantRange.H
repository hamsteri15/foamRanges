#pragma once

#include "constantIterator.H"
#include "dimensionedRange.H"

namespace FoamRanges {

template <class Value, class GeoMesh>
struct DimensionedConstantRange
    : public DimensionedRange<constantIterator<Value>, GeoMesh> {

    // using iterator        = constantIterator<Value>;
    using parent = DimensionedRange<constantIterator<Value>, GeoMesh>;
    using typename parent::difference_type;
    using typename parent::value_type;

    constexpr DimensionedConstantRange(value_type                c,
                                       difference_type           size,
                                       const Foam::dimensionSet& dimensions,
                                       const GeoMesh&            mesh,
                                       const Foam::word&         name)
        : parent(makeConstantRange(c, size), dimensions, mesh, name) {}
};

template <typename Value, typename Size, typename GeoMesh>
constexpr auto makeDimConstantRange(
    const Foam::dimensioned<Value>& v, Size size, const GeoMesh& mesh) {

    return DimensionedConstantRange<Value, GeoMesh>(
        v.value(), size, v.dimensions(), mesh, v.name());
}

} // namespace FoamRanges