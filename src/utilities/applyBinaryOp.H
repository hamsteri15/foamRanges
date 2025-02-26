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
    FoamRanges::detail::ApplyBinaryOp

Description
    Wrapper class for a binary operation of a dereferenced zip iterator which
    returns a tuple.

SourceFiles
    

\*---------------------------------------------------------------------------*/

#pragma once

#include "tuple.H"

namespace FoamRanges
{
namespace detail
{

template<typename BinaryOp>
struct ApplyBinaryOp
{
    BinaryOp op;

    inline constexpr ApplyBinaryOp() = default;

    inline constexpr ApplyBinaryOp(const BinaryOp& f)
    :
        op(f)
    {}

    template<typename Tuple>
    inline constexpr auto operator()(Tuple&& t) const
    {
        return op
        (
            get<0>(std::forward<Tuple>(t)),
            get<1>(std::forward<Tuple>(t))
        );
    }
};

} // namespace detail
} // namespace FoamRanges