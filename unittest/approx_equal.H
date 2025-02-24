#pragma once

#include "fieldTypes.H"
#include "DimensionedField.H"
#include "volFields.H"

namespace Foam{


static constexpr scalar TOLERANCE = 1e-10;

static inline bool approx_equal(scalar l, scalar r)
{
    return std::abs(l - r) < TOLERANCE;
}

template<class Type>
static inline bool approx_equal(const Field<Type>& lhs, const Field<Type>& rhs)
{

    return std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end(), [](const Type& l, const Type& r)
    {
        return approx_equal(l, r);
    });
}

template<class Type, class GeoMesh>
static inline bool approx_equal(const DimensionedField<Type, GeoMesh>& lhs, const DimensionedField<Type, GeoMesh>& rhs)
{
    if (lhs.dimensions() != rhs.dimensions())
    {
        Info << "Dimensions are not equal" <<lhs.dimensions() << " " << rhs.dimensions() <<  endl;
        return false;
    }
    if (lhs.name() != rhs.name())
    {
        Info << "Names are not equal" << lhs.name() << " " << rhs.name() << endl;
        return false;
    }
    if (!approx_equal(lhs.primitiveField(), rhs.primitiveField()))
    {
        Info << "Primitive fields are not equal" << endl;
        return false;
    }
    return true;
}

template<template<class> class FieldType, class Type>
static inline bool approx_equal(const FieldField<FieldType, Type>& lhs, const FieldField<FieldType, Type>& rhs)
{
    forAll(lhs, i)
    {
        if (!approx_equal(lhs[i], rhs[i]))
        {
            Info << "Boundary field at index " << i << " is not equal" << endl;
            return false;
        }
    };
    return true;
}


template<class Type>
static inline bool approx_equal(const VolField<Type>& lhs, const VolField<Type>& rhs)
{
    if (!approx_equal(lhs.internalField(), rhs.internalField()))
    {
        return false;
    }

    return approx_equal(lhs.boundaryField(), rhs.boundaryField());

}

}
