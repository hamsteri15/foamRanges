/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
    \\  /    A nd           | Version:  10
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
    FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

Description
    Functors for various mathematical operations.

SourceFiles
    functors.C

\*---------------------------------------------------------------------------*/

#pragma once

#include "dimensionSet.H"
#include "word.H"
#include "scalar.H"
#include "isAddable.H"

namespace Foam
{


struct PlusOne
{
    constexpr int operator()(Foam::scalar e) const { return e + 1; }

    auto operator()(const Foam::dimensionSet& d) const { return d; }

    auto operator()(const Foam::word& w) const { return w; }
};

struct Mag
{
    template<class T>
    inline auto operator()(const T& t) const { return Foam::mag(t); }

    inline auto operator()(const Foam::dimensionSet& d) const { return d; }

    inline auto operator()(const Foam::word& t) const
    {
        return "mag(" + t + ')';
    }
};


struct Plus
{
    template<class Lhs, class Rhs>
    inline auto operator()(const Lhs& lhs, const Rhs& rhs) const
    {
        static_assert(is_addable_v<Lhs, Rhs>, "Error: Plus::operator() requires Lhs + Rhs to be valid.");
        return lhs + rhs;
    }

    inline auto operator()(const Foam::word& lhs, const Foam::word& rhs) const
    {
        return "(" + lhs + '+' + rhs + ')';
    }
};



struct Multiplies
{
    template<class Lhs, class Rhs>
    inline auto operator()(const Lhs& lhs, const Rhs& rhs) const
    {
        return lhs * rhs;
    }

    inline auto operator()(const Foam::word& lhs, const Foam::word& rhs) const
    {
        return "(" + lhs + '*' + rhs + ')';
    }
};

} // namespace Foam