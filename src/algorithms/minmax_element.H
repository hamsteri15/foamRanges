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

\*---------------------------------------------------------------------------*/

#pragma once

#include "compilerPrimitives.H"
#include "Range.H"
#include <algorithm>

#ifdef __NVIDIA_COMPILER__
#include <thrust/extrema.h>
#endif

namespace FoamRanges
{

#ifdef __NVIDIA_COMPILER__

template<class Rng, class BinaryOp>
constexpr auto min_element(const Rng& rng, const BinaryOp& op)
{
    return *thrust::min_element(rng.begin(), rng.end(), op);
}

template<class Rng, class BinaryOp>
constexpr auto max_element(const Rng& rng, const BinaryOp& op)
{
    return *thrust::max_element(rng.begin(), rng.end(), op);
}


#else

template<class Rng, class BinaryOp>
constexpr auto min_element(const Rng& rng, const BinaryOp& op)
{
    return *std::min_element(rng.begin(), rng.end(), op);
}

template<class Rng, class BinaryOp>
constexpr auto max_element(const Rng& rng, const BinaryOp& op)
{
    return *std::max_element(rng.begin(), rng.end(), op);
}

#endif

} // namespace FoamRanges