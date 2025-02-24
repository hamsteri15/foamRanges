#pragma once

#include "constant_iterator.hpp"
#include "dimensioned_constant_range.hpp"
#include "geometric_range.hpp"
#include "md_constant_range.hpp"
#include "md_range.hpp"

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
constexpr auto make_geometric_constant_range(const Foam::dimensioned<Value>& v,
                                             const GeoMesh& mesh) {

    return GeometricConstantRange<Value, GeoMesh>(
        make_dimensioned_constant_range(v, mesh.size(), mesh),
        make_md_constant_range(v.value()));
}

} // namespace FoamRanges