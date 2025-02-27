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

#include "traits.H"
#include "constantRange.H"
#include "rngTransform.H"
#include "dimensionedConstantRange.H"
#include "dimensionedTransform.H"
#include "geometricConstantRange.H"
#include "geometricTransform.H"
#include "mdConstantRange.H"
#include "mdTransform.H"
#include "ensureDimensioned.H"

// Note! These we expose to the Foam namespace
namespace Foam
{

template<class T1, class T2, class BinaryOp>
constexpr auto rngPromotingTransform(const T1& lhs, const T2& rhs, BinaryOp f)
{
    if constexpr (BothFoamField_v<T1, T2>)
    {
        return FoamRanges::rngTransform(lhs, rhs, f);
    }
    else if constexpr (IsFoamField_v<T1>)
    {
        return FoamRanges::rngTransform
        (
            lhs,
            FoamRanges::makeConstantRange(rhs, std::size(lhs)),
            f
        );
    }
    else
    {
        return FoamRanges::rngTransform
        (
            FoamRanges::makeConstantRange(lhs, std::size(rhs)),
            rhs,
            f
        );
    }
}

template<class T1, class T2, class BinaryOp>
constexpr auto dimrngPromotingTransform(const T1& lhs, const T2& rhs, const BinaryOp& f)
{
    using namespace FoamRanges;

    if constexpr (BothDimensionedField_v<T1, T2>)
    {
        return dimensionedTransform(lhs, rhs, f);
    }
    else if constexpr (IsFoamDimensionedField_v<T1>)
    {
        return dimensionedTransform
        (
            lhs,
            makeDimConstantRange
            (
                EnsureDimensioned<T2>(rhs).type,
                lhs.size(),
                getGeoMesh(lhs)
            ),
            f
        );
    }
    else
    {
        return dimensionedTransform
        (
            makeDimConstantRange
            (
                EnsureDimensioned<T1>(lhs).type,
                rhs.size(),
                getGeoMesh(rhs)
            ),
            rhs,
            f
        );
    }
}

template<class T1, class T2, class BinaryOp>
constexpr auto mdrngPromotingTransform(T1&& lhs, T2&& rhs, BinaryOp&& f)
{
    using namespace FoamRanges;

    if constexpr (BothFieldField_v<std::decay_t<T1>, std::decay_t<T2>>)
    {
        return mdTransform
        (
            std::forward<T1>(lhs),
            std::forward<T2>(rhs),
            std::forward<BinaryOp>(f)
        );
    }
    else if constexpr (IsFoamFieldField_v<std::decay_t<T1>>)
    {
        return mdTransform
        (
            std::forward<T1>(lhs),
            makeMdConstantRange(rhs),
            std::forward<BinaryOp>(f)
        );
    }
    else
    {
        return mdTransform
        (
            makeMdConstantRange(lhs),
            std::forward<T2>(rhs),
            std::forward<BinaryOp>(f)
        );
    }
}

template<class T1, class T2, class BinaryOp>
constexpr auto georngPromotingTransform(const T1& lhs, const T2& rhs, const BinaryOp& f)
{
    using namespace FoamRanges;

    if constexpr (BothGeometricField_v<T1, T2>)
    {
        return geometricTransform(lhs, rhs, f);
    }
    else if constexpr (IsFoamGeometricField_v<T1>)
    {
        return geometricTransform
        (
            lhs,
            makeGeoConstantRange
            (
                EnsureDimensioned<T2>(rhs).type,
                getGeoMesh(lhs)
            ),
            f
        );
    }
    else
    {
        return geometricTransform
        (
            makeGeoConstantRange
            (
                EnsureDimensioned<T1>(lhs).type,
                getGeoMesh(rhs)
            ),
            rhs,
            f
        );
    }
}

} // namespace Foam