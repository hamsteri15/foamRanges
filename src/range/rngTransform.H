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
    FoamRanges::rngTransform

Description
    Functions for creating transform ranges.

SourceFiles


\*---------------------------------------------------------------------------*/

#pragma once

#include "applyBinaryOp.H"
#include "transformRange.H"
#include "zipRange.H"

namespace FoamRanges
{

template<class Range_t, class UnaryOp>
inline constexpr auto rngTransform(const Range_t& rng, const UnaryOp& f)
{
    return makeTransformRange(rng, f);
}

template<class Range1_t, class Range2_t, class BinaryOp>
inline constexpr auto rngTransform
(
    const Range1_t& rng1,
    const Range2_t& rng2,
    const BinaryOp& f
)
{
    return makeTransformRange
    (
        makeZipRange(rng1, rng2),
        detail::ApplyBinaryOp<BinaryOp>(f)
    );
}

} // namespace FoamRanges