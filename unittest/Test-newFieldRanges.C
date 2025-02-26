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

auto func2(const NewVolScalarField& f)
{
    return plus(f, f);
}

}


TEST_CASE("Test-newFieldRanges")
{
    using namespace Foam;

    Foam::argList& args = getFoamArgs();
    FieldReader reader(args);



    NewVolScalarField p = toNewField(reader.read_scalarField("p"));
    SECTION("Implicit cast from Rng -> NewGeometricField")
    {
        CHECK(approxEqual(func1(p) , volScalarField(p + p)));
    }
    SECTION("Implicit cast from Rng -> const NewGeometricField&")
    {
        auto rng = func2(plus(p, p));
        CHECK(approxEqual(NewVolScalarField(rng), volScalarField(p + p + p)));
    }

    

}
