#pragma once

#include "primitiveFields.H"
#include "traits.H"
#include "copy.H"
#include "operations.H"
#include "DimensionedField.H"
#include "promotingTransforms.H"

namespace Foam{


//An example of a new GeometricField type which allows construction from ranges
template<class Type, class GeoMesh, template<class> class PrimitiveField>
struct NewGeometricField : public GeometricField<Type, GeoMesh, PrimitiveField>{

    using base_type = GeometricField<Type, GeoMesh, PrimitiveField>;

    //This is required for the range constructor
    using GeoMeshType = GeoMesh;

    //Inherit constructors
    using GeometricField<Type, GeoMesh, PrimitiveField>::GeometricField;

    using Patch = typename base_type::Patch;

    //These are flags which are used to determine the type of field.
    //Ideally, these would be type traits but because every field type is
    //UList -> Field -> DimensionedField -> GeometricField, finding the right traits
    //becomes tricky.
    static constexpr bool is_foam_field = false;
    static constexpr bool is_foam_field_field = false;
    static constexpr bool is_foam_dimensioned_field = false;
    static constexpr bool is_foam_geometric_field = true;

    //Allow construction from ranges
    template<class Rng, std::enable_if_t<IsFoamGeometricField_v<Rng>, int> = 0>
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
        Patch::calculatedType()
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





template<class Type, class GeoMesh, template<class> class PrimitiveField>
static inline NewGeometricField<Type, GeoMesh, PrimitiveField> toNewField
(
    const GeometricField<Type, GeoMesh, PrimitiveField>& f
)
{
    return NewGeometricField<Type, GeoMesh, PrimitiveField>(f);
}

template<class Type, class GeoMesh, template<class> class PrimitiveField>
static inline NewGeometricField<Type, GeoMesh, PrimitiveField> toNewField
(
    const tmp<GeometricField<Type, GeoMesh, PrimitiveField>>& f
)
{
    return toNewField(f());
}


///These are the operations for newGeometricFields

template<class GeomRange_t, std::enable_if_t<IsFoamGeometricField_v<GeomRange_t>, bool> = true>
constexpr auto mag2(const GeomRange_t& rng)
{
    return FoamRanges::geometricTransform(rng, Mag{});
}

template<class T1, class T2, std::enable_if_t<AtleastOneGeometricField_v<T1, T2>, bool> = true>
constexpr auto plus(const T1& rng1, const T2& rng2)
{
    return georngPromotingTransform(rng1, rng2, Plus{});
}

template<class T1, class T2, std::enable_if_t<AtleastOneGeometricField_v<T1, T2>, bool> = true>
constexpr auto mult(const T1& rng1, const T2& rng2)
{
    return georngPromotingTransform(rng1, rng2, Multiplies{});
}



template<class Type>
using NewVolField = NewGeometricField<Type, volMesh, Field>;


using NewVolScalarField = NewVolField<scalar>;
using NewVolVectorField = NewVolField<vector>;

} // namespace Foam