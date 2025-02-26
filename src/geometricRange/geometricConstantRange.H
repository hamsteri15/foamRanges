#pragma once

#include "constantIterator.H"
#include "dimensionedConstantRange.H"
#include "geometricRange.H"
#include "mdConstantRange.H"
#include "mdRange.H"

namespace FoamRanges {

template <class Value, class GeoMesh>
struct GeometricConstantRange
    : public GeometricRange<DimensionedConstantRange<Value, GeoMesh>,
                             MdConstantRange<Value>> {

    using base_type = GeometricRange<DimensionedConstantRange<Value, GeoMesh>,
                                      MdConstantRange<Value>>;

    template <class I, class B>
    constexpr GeometricConstantRange(I&& internal, B&& boundary)
        : base_type(std::forward<I>(internal), std::forward<B>(boundary)) {}
};

template <typename Value, typename GeoMesh>
constexpr auto makeGeoConstantRange(const Foam::dimensioned<Value>& v,
                                             const GeoMesh& mesh) {

    return GeometricConstantRange<Value, GeoMesh>(
        makeDimConstantRange(v, mesh.size(), mesh),
        makeMdConstantRange(v.value()));
}

} // namespace FoamRanges