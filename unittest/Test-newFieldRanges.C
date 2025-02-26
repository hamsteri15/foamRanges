#define CATCH_CONFIG_ENABLE_BENCHMARKING
#include "catch.H"
#include "args.H"
#include "fieldReader.H"
#include "geomeshAccess.H"
#include "approxEqual.H"


#include "newField.H"
#include "newGeometricField.H"
#include "operations.H"

#include "dimensionedType.H"
#include "promotingTransforms.H"


namespace Foam{


NewVolScalarField func1(const NewVolScalarField& f)
{
    return plus(f, f);
}

}


TEST_CASE("Test-newFieldRanges")
{
    using namespace Foam;

    Foam::argList& args = getFoamArgs();
    FieldReader reader(args);

    using scalarFieldType = NewField<scalar>;
    using vectorFieldType = NewField<vector>;
    using volScalarFieldType = NewVolScalarField;
    using volVectorFieldType = NewVolVectorField;

    volScalarFieldType p = toNewField(reader.read_scalarField("p"));
    SECTION("Implicit cast from Rng -> NewGeometricField")
    {
        CHECK(approxEqual(func1(p) , volScalarField(p + p)));
    }

    /*
    SECTION("scalarField")
    {
        scalarFieldType f = toNewField(reader.read_scalarField("p").internalField().primitiveField());

    */

}
