#pragma once

#include "DimensionedField.H"

namespace FoamRanges
{

//This is a super dirty hack to specialize the getGeoMesh function for DimensionedField to allow for
//mesh access in Dimensioned ranges. This header needs to be included before any of the dimensioend range headers from FoamRanges.
//In practice this typedef would be available in Foam::DimensionedFields

template<class Type, class GeoMesh>
static inline auto getGeoMesh(const Foam::DimensionedField<Type, GeoMesh>& f)
{
    return GeoMesh(f.mesh());
}


}


