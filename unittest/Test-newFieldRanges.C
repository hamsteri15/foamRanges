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

template<class Rng>
auto func2(const Rng& f)
{
    return plus(f, f);
}

auto func3(const NewVolScalarField& f)
{
    return plus(f, f);
}

} //Namesapce Foam


TEST_CASE("Test-newFieldRanges")
{
    using namespace Foam;

    Foam::argList& args = getFoamArgs();
    FieldReader reader(args);



    const NewVolScalarField p = toNewField(reader.read_scalarField("p"));
    const NewVolVectorField U = toNewField(reader.read_vectorField("U"));

    SECTION("Implicit cast from Rng -> NewGeometricField")
    {
        CHECK(approxEqual(func1(p) , volScalarField(p + p)));
    }

    SECTION("Implicit cast from Range -> const NewVolScalarField&")
    {
        auto rng1 = plus(p, p);
        auto rng = func1(rng1);
        CHECK(approxEqual(NewVolScalarField(rng), volScalarField((p + p) + (p + p))));
    }

    SECTION("No casts, take a range and return a range")
    {
        auto rng = func2(plus(p, p));
        CHECK(approxEqual(NewVolScalarField(rng), volScalarField((p + p) + (p + p))));
    }

    
    SECTION("Implicit cast from Range -> const NewVolScalarField& and return a range")
    {
        //TODO: This should work...
        auto rng1 = plus(p, p);
        auto rng = func3(rng1);
        CHECK(approxEqual(NewVolScalarField(rng), volScalarField((p + p) + (p + p))));
    }

    SECTION("Error messages")
    {
        //volScalarField ret = p + p + U;

        //NewVolScalarField ret2 = plus(plus(p, p), U);

    }



    

}
