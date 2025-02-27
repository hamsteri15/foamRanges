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
#include <numeric>
#include <functional>

#ifdef __NVIDIA_COMPILER__
#include <thrust/reduce.h>
#endif

namespace FoamRanges
{

#ifdef __NVIDIA_COMPILER__

template<class Rng, class T, class BinaryOp>
constexpr auto reduce(const Rng& rng, T init, BinaryOp op = thrust::plus<T>())
{
    return thrust::reduce(rng.begin(), rng.end(), init, op);
}

#else

template<class Rng, class T, class BinaryOp>
constexpr auto reduce(const Rng& rng, T init, BinaryOp op = std::plus<T>())
{
    return std::reduce(rng.begin(), rng.end(), init, op);
}

#endif

} // namespace FoamRanges