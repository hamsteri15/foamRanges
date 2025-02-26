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
    FoamRanges::GeometricConstantRange

Description
    Generic range type for constant geometric values with dimensions.

SourceFiles
    

\*---------------------------------------------------------------------------*/

#pragma once

#include "constantIterator.H"
#include "dimensionedConstantRange.H"
#include "geometricRange.H"
#include "mdConstantRange.H"
#include "mdRange.H"

namespace FoamRanges
{

template<class Value, class GeoMesh>
struct GeometricConstantRange
:
    public GeometricRange<DimensionedConstantRange<Value, GeoMesh>, MdConstantRange<Value>>
{
    using base_type 
        = GeometricRange<DimensionedConstantRange<Value, GeoMesh>, MdConstantRange<Value>>;

    template<class I, class B>
    constexpr GeometricConstantRange(I&& internal, B&& boundary)
    :
        base_type(std::forward<I>(internal), std::forward<B>(boundary))
    {}
};

template<typename Value, typename GeoMesh>
constexpr auto makeGeoConstantRange(const Foam::dimensioned<Value>& v, const GeoMesh& mesh)
{
    return GeometricConstantRange<Value, GeoMesh>
    (
        makeDimConstantRange(v, mesh.size(), mesh),
        makeMdConstantRange(v.value())
    );
}

} // namespace FoamRanges