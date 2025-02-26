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
    FoamRanges::MdRange

Description
    Generic range type for multi-dimensional values.

SourceFiles
    

\*---------------------------------------------------------------------------*/

#pragma once

#include "Range.H"

namespace FoamRanges
{

struct MdRange
{
    static constexpr bool is_foam_field             = false;
    static constexpr bool is_foam_field_field       = true;
    static constexpr bool is_foam_dimensioned_field = false;
    static constexpr bool is_foam_geometric_field   = false;
};

} // namespace FoamRanges