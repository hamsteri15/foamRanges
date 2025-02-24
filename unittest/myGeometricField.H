#pragma once

#include "primitiveFields.H"
#include "traits.hpp"
#include "copy.hpp"
#include "operations.H"
#include "DimensionedField.H"
#include "promoting_transforms.hpp"

namespace Foam{



template<class GeomRange_t, std::enable_if_t<IsGeometricRange_v<GeomRange_t>, bool> = true>
constexpr auto mag2(const GeomRange_t& rng)
{
    return FoamRanges::geometric_transform(rng, Mag{});
}

template<class T1, class T2, std::enable_if_t<AtleastOneGeometricRange_v<T1, T2>, bool> = true>
constexpr auto plus(const T1& rng1, const T2& rng2)
{
    return detail::georng_promoting_transform(rng1, rng2, Plus{});
}

template<class T1, class T2, std::enable_if_t<AtleastOneGeometricRange_v<T1, T2>, bool> = true>
constexpr auto mult(const T1& rng1, const T2& rng2)
{
    return detail::georng_promoting_transform(rng1, rng2, Multiplies{});
}



template<class Type, template<class> class PatchField, class GeoMesh>
struct NewGeometricField : public GeometricField<Type, PatchField, GeoMesh>{

    using base_type = GeometricField<Type, PatchField, GeoMesh>;

    //This is required for the range constructor
    using GeoMeshType = GeoMesh;

    //Inherit constructors
    using GeometricField<Type, PatchField, GeoMesh>::GeometricField;

    static constexpr bool is_foam_field = false;
    static constexpr bool is_foam_field_field = false;
    static constexpr bool is_foam_dimensioned_field = false;
    static constexpr bool is_foam_geometric_field = true;

    //Allow construction from ranges
    template<class Rng, std::enable_if_t<IsGeometricRange_v<Rng>, int> = 0>
    NewGeometricField(const Rng& rng) :
    base_type
    (
        IOobject
        (
            rng.name(),
            rng.mesh()().thisDb().time().name(),
            rng.mesh()().thisDb(),
            IOobject::NO_READ,
            IOobject::NO_WRITE,
            rng.mesh()().thisDb().cacheTemporaryObject(rng.name())
        ),
        rng.mesh()(), //range stores a GeoMesh, we get the fvMesh from it
        rng.dimensions(),
        PatchField<Type>::calculatedType()
    )
    {
        FoamRanges::copy(rng.primitiveField(), this->primitiveFieldRef());

        auto bfield = rng.boundaryField();
        auto& mybfield = this->boundaryFieldRef();

        //This is ok because in the tests PatchField is always either fvPatchField or fvsPatchField
        //In production we would simply implement geometricBoundaryField to do the right thing and call
        // this->boundaryFieldRef() = rng.boundaryField();
        forAll(mybfield, i)
        {

            if (mybfield[i].assignable())
            {
                FoamRanges::copy(bfield[i], mybfield[i]);
            }
        }



    }

};





template<class Type, template<class> class PatchField, class GeoMesh>
static inline NewGeometricField<Type, PatchField, GeoMesh> toNewField
(
    const GeometricField<Type, PatchField, GeoMesh>& f
)
{
    return NewGeometricField<Type, PatchField, GeoMesh>(f);
}

template<class Type, template<class> class PatchField, class GeoMesh>
static inline NewGeometricField<Type, PatchField, GeoMesh> toNewField
(
    const tmp<GeometricField<Type, PatchField, GeoMesh>>& f
)
{
    return toNewField(f());
}

template<class Type>
using MyVolField = NewGeometricField<Type, fvPatchField, volMesh>;


using MyVolScalarField = MyVolField<scalar>;
using MyVolVectorField = MyVolField<vector>;

} // namespace Foam