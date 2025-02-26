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

#include <tuple> // std::tuple
#include "compilerPrimitives.H"

#ifdef __NVIDIA_COMPILER__
#include <thrust/tuple.h>
#endif

namespace FoamRanges
{

#ifdef __NVIDIA_COMPILER__

template<class... Types>
using Tuple = thrust::tuple<Types...>;

using thrust::get;

template<class T>
using tuple_size = thrust::tuple_size<T>;

template<class... Types>
inline constexpr auto adl_make_tuple(Types&&... args)
{
    return thrust::make_tuple(std::forward<Types>(args)...);
}

#else

template<class... Types>
using Tuple = std::tuple<Types...>;

template<class T>
using tuple_size = std::tuple_size<T>;

using std::get;

template<class... Types>
inline constexpr auto adl_make_tuple(Types&&... args)
{
    return std::make_tuple(std::forward<Types>(args)...);
}

#endif

} // namespace FoamRanges