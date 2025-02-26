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
    Foam::EnsureDimensioned

Description
    Template struct to ensure a type is dimensioned. Promotes scalars to
    dimensioned scalars.

SourceFiles
    

\*---------------------------------------------------------------------------*/

#pragma once

#include "dimensionSet.H"

namespace Foam
{

// Note! This overload also promotes scalars to dimensioned scalars. Slightly
// dangerous but necessary for some operations
template<class T>
struct EnsureDimensioned
{
    constexpr EnsureDimensioned(const T& t)
    :
        type(t)
    {}

    Foam::dimensioned<T> type;
};

template<class T>
struct EnsureDimensioned<Foam::dimensioned<T>>
{
    constexpr EnsureDimensioned(const Foam::dimensioned<T>& t)
    :
        type(t)
    {}

    Foam::dimensioned<T> type;
};

} // namespace Foam