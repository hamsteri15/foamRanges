#pragma once

#include "constant_iterator.hpp"
#include "dimensioned_range.hpp"

namespace FoamRanges {

template <class Value, class GeoMesh>
struct DimensionedConstantRange
    : public DimensionedRange<constant_iterator<Value>, GeoMesh> {

    // using iterator        = constant_iterator<Value>;
    using parent = DimensionedRange<constant_iterator<Value>, GeoMesh>;
    using typename parent::difference_type;
    using typename parent::value_type;

    constexpr DimensionedConstantRange(value_type                c,
                                       difference_type           size,
                                       const Foam::dimensionSet& dimensions,
                                       const GeoMesh&            mesh,
                                       const Foam::word&         name)
        : parent(make_constant_range(c, size), dimensions, mesh, name) {}
};

template <typename Value, typename Size, typename GeoMesh>
constexpr auto make_dimensioned_constant_range(
    const Foam::dimensioned<Value>& v, Size size, const GeoMesh& mesh) {

    return DimensionedConstantRange<Value, GeoMesh>(
        v.value(), size, v.dimensions(), mesh, v.name());
}

} // namespace FoamRanges