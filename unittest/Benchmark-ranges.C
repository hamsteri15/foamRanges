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



TEST_CASE("Benchmark-Ranges")
{
    using namespace Foam;

    Foam::argList& args = getFoamArgs();
    FieldReader reader(args);

    using scalarFieldType = NewField<scalar>;
    using vectorFieldType = NewField<vector>;
    using volScalarFieldType = NewVolScalarField;
    using volVectorFieldType = NewVolVectorField;

    SECTION("scalarField")
    {
        scalarFieldType f = toNewField(reader.read_scalarField("p").internalField().primitiveField());

        BENCHMARK("WARMUP") {
            return scalarFieldType(f + f);
        };

        BENCHMARK("scalar * f (old)") {
            return scalarFieldType(scalar(2) * f);
        };
        BENCHMARK("scalar * f (new)") {
            return scalarFieldType(mult(scalar(2), f));
        };

        BENCHMARK("f + f (old)") {
            return scalarFieldType(f + f);
        };

        BENCHMARK("f + f (new)") {
            return scalarFieldType(plus(f, f));
        };

        BENCHMARK("f + f + f (old)") {
            return scalarFieldType(f + f + f);
        };

        BENCHMARK("f + f + f (new)") {
            return scalarFieldType(plus(plus(f, f), f));
        };

        BENCHMARK("f + f + f + f (old)") {
            return scalarFieldType(f + f + f + f);
        };

        BENCHMARK("f + f + f + f (new)") {
            return scalarFieldType(plus(plus(plus(f, f), f), f));
        };

    }

    SECTION("vectorField")
    {
        vectorFieldType f = toNewField(reader.read_vectorField("U").internalField().primitiveField());

        BENCHMARK("WARMUP") {
            return vectorFieldType(f + f);
        };

        BENCHMARK("scalar * f (old)") {
            return vectorFieldType(scalar(2) * f);
        };
        BENCHMARK("scalar * f (new)") {
            return vectorFieldType(mult(scalar(2), f));
        };

        BENCHMARK("f + f (old)") {
            return vectorFieldType(f + f);
        };

        BENCHMARK("f + f (new)") {
            return vectorFieldType(plus(f, f));
        };

        BENCHMARK("f + f + f (old)") {
            return vectorFieldType(f + f + f);
        };

        BENCHMARK("f + f + f (new)") {
            return vectorFieldType(plus(plus(f, f), f));
        };

        BENCHMARK("f + f + f + f (old)") {
            return vectorFieldType(f + f + f + f);
        };

        BENCHMARK("f + f + f + f (new)") {
            return vectorFieldType(plus(plus(plus(f, f), f), f));
        };

    }



    SECTION("volScalarField")
    {
        volScalarFieldType f = toNewField(reader.read_scalarField("p"));

        BENCHMARK("WARMUP") {
            return volScalarFieldType(f + f);
        };

        BENCHMARK("scalar * p (old)") {
            return volScalarFieldType(scalar(2) * f);
        };
        BENCHMARK("scalar * p (new)") {
            return volScalarFieldType(mult(scalar(2), f));
        };

        BENCHMARK("p + p (old)") {
            return volScalarFieldType(f + f);
        };

        BENCHMARK("p + p (new)") {
            return volScalarFieldType(plus(f, f));
        };

        BENCHMARK("p + p + p (old)") {
            return volScalarFieldType(f + f + f);
        };

        BENCHMARK("p + p + p (new)") {
            return volScalarFieldType(plus(plus(f, f), f));
        };

        BENCHMARK("p + p + p + p (old)") {
            return volScalarFieldType(f + f + f + f);
        };

        BENCHMARK("p + p + p + p (new)") {
            return volScalarFieldType(plus(plus(plus(f, f), f), f));
        };

    }

    SECTION("volVectorFieldType")
    {
        volVectorFieldType f = toNewField(reader.read_vectorField("U"));

        BENCHMARK("WARMUP") {
            return volVectorFieldType(f + f);
        };

        BENCHMARK("scalar * U (old)") {
            return volVectorFieldType(scalar(2) * f);
        };
        BENCHMARK("scalar * U (new)") {
            return volVectorFieldType(mult(scalar(2), f));
        };

        BENCHMARK("U + U (old)") {
            return volVectorFieldType(f + f);
        };

        BENCHMARK("U + U (new)") {
            return volVectorFieldType(plus(f, f));
        };

        BENCHMARK("U + U + U (old)") {
            return volVectorFieldType(f + f + f);
        };

        BENCHMARK("U + U + U (new)") {
            return volVectorFieldType(plus(plus(f, f), f));
        };

        BENCHMARK("U + U + U + U (old)") {
            return volVectorFieldType(f + f + f + f);
        };

        BENCHMARK("U + U + U + U (new)") {
            return volVectorFieldType(plus(plus(plus(f, f), f), f));
        };

    }


    SECTION("Mixed volField operations")
    {

        volScalarFieldType p = toNewField(reader.read_scalarField("p"));
        volVectorFieldType U = toNewField(reader.read_vectorField("U"));
        BENCHMARK("WARMUP") {
            return volVectorFieldType((mag(p) * U) + (mag(p) * U));
        };

        BENCHMARK("mag(p) * U (old)") {
            return volVectorFieldType(mag(p) * U);
        };
        BENCHMARK("mag(p) * U (new)") {
            return volVectorFieldType(mult(mag2(p), U));
        };

        BENCHMARK("mag(p) * U + mag(p) * U (old)") {
            return volVectorFieldType((mag(p) * U) + (mag(p) * U));
        };

        BENCHMARK("mag(p) * U + mag(p) * U (new)") {
            return volVectorFieldType(plus(mult(mag2(p), U), mult(mag2(p), U)));
        };

    }

}
