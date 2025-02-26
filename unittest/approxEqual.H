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
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

Description
    Utility functions for approximate equality checks on fields.

SourceFiles

\*---------------------------------------------------------------------------*/

#pragma once

#include "fieldTypes.H"
#include "DimensionedField.H"
#include "volFields.H"

namespace Foam
{

static constexpr scalar TOLERANCE = 1e-10;

static inline bool approxEqual(scalar l, scalar r)
{
    return std::abs(l - r) < TOLERANCE;
}

template<class Type>
static inline bool approxEqual(const Field<Type>& lhs, const Field<Type>& rhs)
{
    return std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end(),
        [](const auto& l, const auto& r)
        {
            return approxEqual(l, r);
        });
}

template<class Type, class GeoMesh>
static inline bool approxEqual(
    const DimensionedField<Type, GeoMesh>& lhs,
    const DimensionedField<Type, GeoMesh>& rhs)
{
    if (lhs.dimensions() != rhs.dimensions())
    {
        Info << "Dimensions are not equal" << lhs.dimensions() << " "
             << rhs.dimensions() << endl;
        return false;
    }
    if (lhs.name() != rhs.name())
    {
        Info << "Names are not equal" << lhs.name() << " " << rhs.name() << endl;
        return false;
    }
    if (!approxEqual(lhs.primitiveField(), rhs.primitiveField()))
    {
        Info << "Primitive fields are not equal" << endl;
        return false;
    }
    return true;
}

template<template<class> class FieldType, class Type>
static inline bool approxEqual(
    const FieldField<FieldType, Type>& lhs,
    const FieldField<FieldType, Type>& rhs)
{
    forAll(lhs, i)
    {
        if (!approxEqual(lhs[i], rhs[i]))
        {
            Info << "Boundary field at index " << i << " is not equal" << endl;
            return false;
        }
    }
    return true;
}

template<class Type>
static inline bool approxEqual(const VolField<Type>& lhs, const VolField<Type>& rhs)
{
    if (!approxEqual(lhs.internalField(), rhs.internalField()))
    {
        return false;
    }

    return approxEqual(lhs.boundaryField(), rhs.boundaryField());
}

} // namespace Foam
