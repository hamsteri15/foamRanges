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

#include <type_traits>
#include <iterator>
#include "concepts.H"


namespace Foam
{

class surfaceMesh;
class volMesh;



///////////////////////////////////////////////////////////////////////////////////

template<typename T, typename = void>
struct IsRange
:
    public std::false_type
{};

template<typename T>
struct IsRange<T, std::void_t<decltype(std::declval<T>().begin()), decltype(std::declval<T>().end())>>
:
    public std::true_type
{};

template<typename T>
constexpr bool IsRange_v = IsRange<T>::value;

///////////////////////////////////////////////////////////////////////////////////


template<typename T, typename = void>
struct IsIterator
:
    public std::false_type
{};

template<typename T>
struct IsIterator<T, std::void_t<typename std::iterator_traits<T>::iterator_category>>
:
    std::true_type
{};

template<typename T>
constexpr bool IsIterator_v = IsIterator<T>::value;

///////////////////////////////////////////////////////////////////////////////////

template<class T, typename = void>
struct IsFoamField
:
    public std::false_type
{};

template<class T>
struct IsFoamField<T, std::enable_if_t<T::is_foam_field>>
:
    public std::true_type
{};

template<typename T>
constexpr bool IsFoamField_v = IsFoamField<T>::value;

///////////////////////////////////////////////////////////////////////////////////

template<class T1, class T2, typename = void>
struct BothFoamField
:
    public std::false_type
{};

template<class T1, class T2>
struct BothFoamField<T1, T2, std::enable_if_t<IsFoamField_v<T1> && IsFoamField_v<T2>>>
:
    public std::true_type
{};

template<typename T1, typename T2>
constexpr bool BothFoamField_v = BothFoamField<T1, T2>::value;

///////////////////////////////////////////////////////////////////////////////////

template<class T1, class T2, typename = void>
struct AtleastOneFoamField
:
    public std::false_type
{};

template<class T1, class T2>
struct AtleastOneFoamField<T1, T2, std::enable_if_t<IsFoamField_v<T1> || IsFoamField_v<T2>>>
:
    public std::true_type
{};

template<typename T1, typename T2>
constexpr bool AtleastOneFoamField_v = AtleastOneFoamField<T1, T2>::value;

///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////

template<class T, typename = void>
struct IsFoamDimensionedField
:
    public std::false_type
{};

// TODO: Fix
template<class T>
struct IsFoamDimensionedField<T, std::enable_if_t<T::is_foam_dimensioned_field>>
:
    public std::true_type
{};

template<typename T>
constexpr bool IsFoamDimensionedField_v = IsFoamDimensionedField<T>::value;

///////////////////////////////////////////////////////////////////////////////////

template<class T1, class T2, typename = void>
struct AtleastOneDimensionedField
:
    public std::false_type
{};

template<class T1, class T2>
struct AtleastOneDimensionedField<T1, T2, std::enable_if_t<IsFoamDimensionedField_v<T1> || IsFoamDimensionedField_v<T2>>>
:
    public std::true_type
{};

template<typename T1, typename T2>
constexpr bool AtleastOneDimensionedField_v = AtleastOneDimensionedField<T1, T2>::value;

///////////////////////////////////////////////////////////////////////////////////

template<class T1, class T2, typename = void>
struct BothDimensionedField
:
    public std::false_type
{};

template<class T1, class T2>
struct BothDimensionedField<T1, T2, std::enable_if_t<IsFoamDimensionedField_v<T1> && IsFoamDimensionedField_v<T2>>>
:
    public std::true_type
{};

template<typename T1, typename T2>
constexpr bool BothDimensionedField_v = BothDimensionedField<T1, T2>::value;

///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////

template<class T, typename = void>
struct IsFoamFieldField
:
    public std::false_type
{};

// TODO: Fix
template<class T>
struct IsFoamFieldField<T, std::enable_if_t<T::is_foam_field_field>>
:
    public std::true_type
{};

template<typename T>
constexpr bool IsFoamFieldField_v = IsFoamFieldField<T>::value;

///////////////////////////////////////////////////////////////////////////////////

template<class T1, class T2, typename = void>
struct AtleastOneFieldField
:
    public std::false_type
{};

template<class T1, class T2>
struct AtleastOneFieldField<T1, T2, std::enable_if_t<IsFoamFieldField_v<T1> || IsFoamFieldField_v<T2>>>
:
    public std::true_type
{};

template<typename T1, typename T2>
constexpr bool AtleastOneFieldField_v = AtleastOneFieldField<T1, T2>::value;

///////////////////////////////////////////////////////////////////////////////////

template<class T1, class T2, typename = void>
struct BothFieldField
:
    public std::false_type
{};

template<class T1, class T2>
struct BothFieldField<T1, T2, std::enable_if_t<IsFoamFieldField_v<T1> && IsFoamFieldField_v<T2>>>
:
    public std::true_type
{};

template<typename T1, typename T2>
constexpr bool BothFieldField_v = BothFieldField<T1, T2>::value;

///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////

template<class T, typename = void>
struct IsFoamGeometricField
:
    public std::false_type
{};

// TODO: Fix
template<class T>
struct IsFoamGeometricField<T, std::enable_if_t<T::is_foam_geometric_field>>
:
    public std::true_type
{};

template<typename T>
constexpr bool IsFoamGeometricField_v = IsFoamGeometricField<T>::value;

///////////////////////////////////////////////////////////////////////////////////

template<typename T1, typename T2>
struct BothGeometricField
:
    std::bool_constant<IsFoamGeometricField_v<T1> && IsFoamGeometricField_v<T2>>
{};

template<typename T1, typename T2>
inline constexpr bool BothGeometricField_v = BothGeometricField<T1, T2>::value;

///////////////////////////////////////////////////////////////////////////////////

template<typename T1, typename T2>
struct AtleastOneGeometricField
:
    std::bool_constant<IsFoamGeometricField_v<T1> || IsFoamGeometricField_v<T2>>
{};

template<typename T1, typename T2>
inline constexpr bool AtleastOneGeometricField_v = AtleastOneGeometricField<T1, T2>::value;

///////////////////////////////////////////////////////////////////////////////////

template<typename, typename = std::void_t<>>
struct DeduceMeshType
{
    using type = void;
};

template<typename T>
struct DeduceMeshType<T, std::void_t<typename T::GeoMeshType>>
{
    using type = typename T::GeoMeshType;
};

template<typename T>
using DeduceMeshType_t = typename DeduceMeshType<T>::type;

template<class T>
constexpr bool IsSurfaceField_v = std::is_same_v<DeduceMeshType_t<T>, surfaceMesh>;

template<class T>
constexpr bool IsVolumeField_v = std::is_same_v<DeduceMeshType_t<T>, volMesh>;

///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////

} // namespace Foam