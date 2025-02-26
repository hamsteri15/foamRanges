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
    FoamRanges::dimensionedTransform

Description
    Functions for creating dimensioned transform ranges.

SourceFiles
    

\*---------------------------------------------------------------------------*/

#pragma once

#include "dimensionedZipRange.H"
#include "dimensionedTransformRange.H"
#include "applyBinaryOp.H"

namespace FoamRanges
{

template<class Rng, class UnaryOp>
constexpr auto dimensionedTransform(const Rng& rng, UnaryOp&& f)
{
    return makeDimensionedTransformRange
    (
        rng,
        std::forward<UnaryOp>(f),
        f(rng.dimensions()),
        f(rng.name())
    );
}

template<class Rng1, class Rng2, class BinaryOp>
constexpr auto dimensionedTransform(const Rng1& rng1, const Rng2& rng2, BinaryOp&& f)
{
    return makeDimensionedTransformRange
    (
        rng1, rng2,
        detail::ApplyBinaryOp<BinaryOp>(std::forward<BinaryOp>(f)),
        f(rng1.dimensions(), rng2.dimensions()),
        f(rng1.name(), rng2.name())
    );
}

} // namespace FoamRanges
