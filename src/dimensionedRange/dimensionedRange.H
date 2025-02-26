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

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

Class
    FoamRanges::DimensionedRange

Description
    Generic range type which stores dimensions.

SourceFiles
    

\*---------------------------------------------------------------------------*/

#pragma once

#include "dimensionSet.H"
#include "Range.H"

namespace FoamRanges
{

template<class Iterator, class GeoMesh>
struct DimensionedRange
:
    public Range<Iterator>
{
    using GeoMeshType = GeoMesh;

    static constexpr bool is_foam_field             = false;
    static constexpr bool is_foam_field_field       = false;
    static constexpr bool is_foam_dimensioned_field = true;
    static constexpr bool is_foam_geometric_field   = false;

    template<class Rng, class Dims, class Mesh, class Name>
    inline constexpr DimensionedRange
    (
        Rng&& range,
        Dims&& dims,
        Mesh&& mesh,
        Name&& name
    )
    :
        Range<Iterator>(std::forward<Rng>(range)),
        m_dimensions(std::forward<Dims>(dims)),
        m_mesh(std::forward<Mesh>(mesh)),
        m_name(std::forward<Name>(name))
    {}

    constexpr const Foam::dimensionSet& dimensions() const
    {
        return m_dimensions;
    }

    constexpr GeoMesh mesh() const
    {
        return m_mesh;
    }

    constexpr const Foam::word& name() const
    {
        return m_name;
    }

    constexpr const Range<Iterator>& primitiveField() const
    {
        return *this;
    }

private:
    Foam::dimensionSet m_dimensions;
    GeoMesh m_mesh;
    Foam::word m_name;
};

template<class DimRange>
constexpr auto getGeoMesh(const DimRange& rng)
{
    using mesh_type = decltype(rng.mesh());
    if constexpr (std::is_same_v<mesh_type, typename DimRange::GeoMeshType>)
    {
        return rng.mesh();
    }
    else
    {
        return typename DimRange::GeoMeshType(rng.mesh());
    }
}

template<class DimRange>
constexpr auto make_dimensioned_range(const DimRange& rng)
{
    using Iterator = decltype(rng.primitiveField().begin());
    using GeoMesh = decltype(getGeoMesh(rng));
    return DimensionedRange<Iterator, GeoMesh>
    (
        rng.primitiveField(),
        rng.dimensions(),
        getGeoMesh(rng),
        rng.name()
    );
}

template<class DimRange>
constexpr auto make_dimensioned_range(DimRange& rng)
{
    using Iterator = decltype(rng.primitiveField().begin());
    using GeoMesh = decltype(getGeoMesh(rng));
    return DimensionedRange<Iterator, GeoMesh>
    (
        rng.primitiveField(),
        rng.dimensions(),
        getGeoMesh(rng),
        rng.name()
    );
}

} // namespace FoamRanges