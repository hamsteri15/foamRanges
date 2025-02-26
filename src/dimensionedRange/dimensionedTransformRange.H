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
    FoamRanges::DimensionedTransformRange

Description
    Generic range type which stores dimensions.

SourceFiles
    

\*---------------------------------------------------------------------------*/

#pragma once

#include "dimensionedRange.H"
#include "transformIterator.H"

namespace FoamRanges
{

template<typename UnaryFunction, typename Iterator, typename GeoMesh>
struct DimensionedTransformRange
:
    public DimensionedRange<
        detail::transformIterator<UnaryFunction, Iterator>,
        GeoMesh
    >
{

    using iterator = detail::transformIterator<UnaryFunction, Iterator>;
    using parent = DimensionedRange<iterator, GeoMesh>;

    template<typename Func, typename Dims, typename Mesh, typename Name>
    inline constexpr DimensionedTransformRange
    (
        Iterator begin,
        Iterator end,
        Func&& f,
        Dims&& dims,
        Mesh&& mesh,
        Name&& name
    )
    :
        parent
        (
            makeRange
            (
                detail::makeTransformIterator(begin, std::forward<Func>(f)),
                detail::makeTransformIterator(end, std::forward<Func>(f))
            ),
            std::forward<Dims>(dims),
            std::forward<Mesh>(mesh),
            std::forward<Name>(name)
        )
    {}
};

template<typename Rng, typename Func, typename Dims, typename Name>
constexpr auto makeDimensionedTransformRange
(
    const Rng& rng,
    Func&& f,
    Dims&& newDims,
    Name&& newName
)
{
    using iterator = decltype(std::begin(rng.primitiveField()));
    using mesh = decltype(getGeoMesh(rng));

    return DimensionedTransformRange<std::decay_t<Func>, iterator, mesh>
    (
        std::begin(rng.primitiveField()),
        std::end(rng.primitiveField()),
        std::forward<Func>(f),
        std::forward<Dims>(newDims),
        getGeoMesh(rng),
        std::forward<Name>(newName)
    );
}

template<typename Rng1, typename Rng2, typename Func, typename Dims, typename Name>
constexpr auto makeDimensionedTransformRange
(
    const Rng1& rng1,
    const Rng2& rng2,
    Func&& f,
    Dims&& newDims,
    Name&& newName
)
{
    return makeDimensionedTransformRange
    (
        makeDimensionedZipRange(rng1, rng2),
        std::forward<Func>(f),
        std::forward<Dims>(newDims),
        std::forward<Name>(newName)
    );
}

} // namespace FoamRanges