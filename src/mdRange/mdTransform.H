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
    the Free Software implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

Class
    FoamRanges::mdTransform

Description
    Functions for creating multi-dimensional transform ranges.

SourceFiles
    

\*---------------------------------------------------------------------------*/

#pragma once

#include "mdTransformRange.H"

namespace FoamRanges
{

template<class Rng, class UnaryOp>
constexpr auto mdTransform(Rng&& rng, UnaryOp&& f)
{
    return make_md_transform_range(std::forward<Rng>(rng), std::forward<UnaryOp>(f));
}

template<class Rng1, class Rng2, class BinaryOp>
constexpr auto mdTransform(Rng1&& rng1, Rng2&& rng2, BinaryOp&& f)
{
    return make_md_transform_range
    (
        std::forward<Rng1>(rng1),
        std::forward<Rng2>(rng2),
        detail::ApplyBinaryOp<BinaryOp>(std::forward<BinaryOp>(f))
    );
}

} // namespace FoamRanges