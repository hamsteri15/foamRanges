#pragma once

#include "mdRange.H"
#include "dimensionedRange.H"

#include "dimensionSet.H"

namespace FoamRanges {

template <class Internal, class Boundary> struct GeometricRange {

    static constexpr bool is_foam_field             = false;
    static constexpr bool is_foam_field_field       = false;
    static constexpr bool is_foam_dimensioned_field = false;
    static constexpr bool is_foam_geometric_field   = true;

    using GeoMeshType = typename Internal::GeoMeshType;
    using iterator    = typename Internal::iterator;
    using value_type  = typename std::iterator_traits<iterator>::value_type;
    using reference   = typename std::iterator_traits<iterator>::reference;

    GeometricRange(Internal&& internal, Boundary&& boundary)
        : m_internal(std::forward<Internal>(internal))
        , m_boundary(std::forward<Boundary>(boundary)) {}

    constexpr const Foam::word& name() const { return m_internal.name(); }
    constexpr const GeoMeshType mesh() const { return getGeoMesh(m_internal); }

    constexpr const Foam::dimensionSet& dimensions() const {
        return m_internal.dimensions();
    }

    // These are always ranges so ok to return by value
    constexpr auto primitiveField() const {
        return m_internal.primitiveField();
    }
    constexpr const Internal& internalField() const { return m_internal; }
    constexpr Boundary boundaryField() const { return m_boundary; }

private:
    Internal m_internal;
    Boundary m_boundary;
};


} // namespace FoamRanges