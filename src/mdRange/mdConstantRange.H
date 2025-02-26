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
    FoamRanges::MdConstantRange

Description
    Generic range type for constant multi-dimensional values.

SourceFiles
    

\*---------------------------------------------------------------------------*/

#pragma once

#include "constantIterator.H"
#include "mdRange.H"

namespace FoamRanges
{

// Note! This class does not allocate upon construction but simply stores the
// input value. This is done because we do not want this class to store array of
// 1D ranges because the number of 1D Ranges in a MdRange is generally not known
// compile time. Using this class as is requires special treatment in the
// operator[] of MdTransformZipRange.
template<typename Value>
struct MdConstantRange
:
    public MdRange
{
    using iterator        = constantIterator<Value>;
    using value_type      = Value;
    using difference_type = std::ptrdiff_t;

    inline constexpr MdConstantRange(value_type c)
    :
        m_value(c)
    {}

    inline constexpr value_type get_value() const
    {
        return m_value;
    }

private:
    const value_type m_value;
};

template<typename Value>
constexpr auto makeMdConstantRange(Value c)
{
    return MdConstantRange<Value>(c);
}

} // namespace FoamRanges