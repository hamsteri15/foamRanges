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
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

Class
    FoamRanges::DimensionedConstantRange

Description
    Generic range type for constant values with dimensions.

SourceFiles
    

\*---------------------------------------------------------------------------*/

#pragma once

#include "constantIterator.H"
#include "dimensionedRange.H"

namespace FoamRanges
{

template<class Value, class GeoMesh>
struct DimensionedConstantRange
:
    public DimensionedRange<constantIterator<Value>, GeoMesh>
{
    using parent = DimensionedRange<constantIterator<Value>, GeoMesh>;
    using typename parent::difference_type;
    using typename parent::value_type;

    constexpr DimensionedConstantRange
    (
        value_type c,
        difference_type size,
        const Foam::dimensionSet& dimensions,
        const GeoMesh& mesh,
        const Foam::word& name
    )
    :
        parent(makeConstantRange(c, size), dimensions, mesh, name)
    {}
};

template<typename Value, typename Size, typename GeoMesh>
constexpr auto makeDimConstantRange
(
    const Foam::dimensioned<Value>& v,
    Size size,
    const GeoMesh& mesh
)
{
    return DimensionedConstantRange<Value, GeoMesh>
    (
        v.value(),
        size,
        v.dimensions(),
        mesh,
        v.name()
    );
}

} // namespace FoamRanges