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
    FoamRanges::GeometricTransformRange

Description
    Generic range type for geometric transform operations.

SourceFiles
    

\*---------------------------------------------------------------------------*/

#pragma once

#include "dimensionedTransformRange.H"
#include "dimensionedZipRange.H"
#include "geometricRange.H"
#include "mdTransformRange.H"

namespace FoamRanges
{

template<class Internal, class Boundary>
struct GeometricTransformRange
:
    public GeometricRange<Internal, Boundary>
{
    constexpr GeometricTransformRange
    (
        Internal&& internal,
        Boundary&& boundary
    )
    :
        GeometricRange<Internal, Boundary>
        (
            std::forward<Internal>(internal),
            std::forward<Boundary>(boundary)
        )
    {}
};

template<typename Function, class GeomRange_t>
constexpr auto make_geometric_transform_range
(
    const GeomRange_t& rng,
    Function f,
    const Foam::dimensionSet& dimensions,
    const Foam::word& name
)
{
    using boundary = decltype(make_md_transform_range(rng.boundaryField(), f));
    using internal = decltype
    (
        makeDimensionedTransformRange
        (
            rng.internalField(),
            f,
            dimensions,
            name
        )
    );

    return GeometricTransformRange<internal, boundary>
    (
        makeDimensionedTransformRange
        (
            rng.internalField(),
            f,
            dimensions,
            name
        ),
        make_md_transform_range(rng.boundaryField(), f)
    );
}

template<typename Function, class Rng1, class Rng2>
constexpr auto make_geometric_transform_range
(
    const Rng1& rng1,
    const Rng2& rng2,
    const Function& f,
    const Foam::dimensionSet& dimensions,
    const Foam::word& name
)
{
    using boundary = decltype
    (
        make_md_transform_range
        (
            rng1.boundaryField(),
            rng2.boundaryField(),
            f
        )
    );
    using internal = decltype
    (
        makeDimensionedTransformRange
        (
            makeDimensionedZipRange
            (
                rng1.internalField(),
                rng2.internalField()
            ),
            f,
            dimensions,
            name
        )
    );

    return GeometricTransformRange<internal, boundary>
    (
        makeDimensionedTransformRange
        (
            makeDimensionedZipRange
            (
                rng1.internalField(),
                rng2.internalField()
            ),
            f,
            dimensions,
            name
        ),
        make_md_transform_range
        (
            rng1.boundaryField(),
            rng2.boundaryField(),
            f
        )
    );
}

} // namespace FoamRanges