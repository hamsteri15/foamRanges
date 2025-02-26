/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
    \\  /    A nd           | Copyright (C) 2011-2024 OpenFOAM Foundation
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

Class
    FoamRanges::GeometricRange

Description
    Generic range type for geometric values with dimensions.

SourceFiles
    

\*---------------------------------------------------------------------------*/

#pragma once

#include "mdRange.H"
#include "dimensionedRange.H"
#include "dimensionSet.H"

namespace FoamRanges
{

template<class Internal, class Boundary>
struct GeometricRange
{
    static constexpr bool is_foam_field             = false;
    static constexpr bool is_foam_field_field       = false;
    static constexpr bool is_foam_dimensioned_field = false;
    static constexpr bool is_foam_geometric_field   = true;

    using GeoMeshType = typename Internal::GeoMeshType;
    using iterator    = typename Internal::iterator;
    using value_type  = typename std::iterator_traits<iterator>::value_type;
    using reference   = typename std::iterator_traits<iterator>::reference;

    GeometricRange(Internal&& internal, Boundary&& boundary)
    :
        m_internal(std::forward<Internal>(internal)),
        m_boundary(std::forward<Boundary>(boundary))
    {}

    constexpr const Foam::word& name() const
    {
        return m_internal.name();
    }

    constexpr const GeoMeshType mesh() const
    {
        return getGeoMesh(m_internal);
    }

    constexpr const Foam::dimensionSet& dimensions() const
    {
        return m_internal.dimensions();
    }

    constexpr const Range<iterator>& primitiveField() const
    {
        return m_internal.primitiveField();
    }

    constexpr const Internal& internalField() const
    {
        return m_internal;
    }

    //Note! By value here to allow for temporary ranges.
    constexpr Boundary boundaryField() const
    {
        return m_boundary;
    }

private:
    Internal m_internal;
    Boundary m_boundary;
};

} // namespace FoamRanges