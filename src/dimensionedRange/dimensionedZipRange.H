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
    FoamRanges::DimensionedZipRange

Description
    Generic range type for zip operations without storing dimensions.

SourceFiles
    

\*---------------------------------------------------------------------------*/

#pragma once

#include "dimensionedRange.H"
#include "zipIterator.H"
#include "zipRange.H"

namespace FoamRanges
{

// Note! Here we do not derive from DimensionedRange since there is no need to
// store name and dimensions for zip ranges. Zip ranges are only constructed as
// temporaries for use in transform ranges of binary operations. This class
// could very well be defined inside the DimensionedTransformRange
template<class Rng1, class Rng2>
class DimensionedZipRange
{

public:

    using GeoMeshType = decltype(getGeoMesh(std::declval<Rng1>()));


    constexpr DimensionedZipRange(const Rng1& rng1, const Rng2& rng2)
    :
        m_rng1(rng1),
        m_rng2(rng2)
    {}

    constexpr auto primitiveField() const
    {
        return makeZipRange(m_rng1.primitiveField(), m_rng2.primitiveField());
    }

    constexpr auto mesh() const
    {
        return getGeoMesh(m_rng1);
    }

private:
    const Rng1& m_rng1;
    const Rng2& m_rng2;
};

template<class Rng1, class Rng2>
constexpr auto makeDimensionedZipRange(const Rng1& rng1, const Rng2& rng2)
{
    return DimensionedZipRange<Rng1, Rng2>(rng1, rng2);
}

} // namespace FoamRanges
