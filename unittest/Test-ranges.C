#define CATCH_CONFIG_ENABLE_BENCHMARKING
#include "catch.H"
#include "args.H"
#include "field_reader.H"
#include "geomesh_access.H"
#include "approx_equal.H"
#include "mock_fields.H"

#include "operations.H"

#include "dimensionedType.H"
#include "promotingTransforms.H"


using namespace Foam;


TEST_CASE("Test-Range"){

    auto f1 = MockField<scalar>({1,2,3});
    const auto f2 = f1;

    SECTION("transform"){

        SECTION("unary")
        {
            auto rng1 = FoamRanges::transform(f1, PlusOne{});
            auto rng2 = FoamRanges::transform(f2, PlusOne{});

            CHECK(MockField<scalar>(rng1) == MockField<scalar>({2, 3, 4}));
            CHECK(MockField<scalar>(rng2) == MockField<scalar>({2, 3, 4}));
        }

        SECTION("binary")
        {
            auto rng1 = FoamRanges::transform(f1, f2, Plus{});
            auto rng2 = FoamRanges::transform(f2, f1, Plus{});

            CHECK(MockField<scalar>(rng1) == MockField<scalar>({2, 4, 6}));
            CHECK(MockField<scalar>(rng2) == MockField<scalar>({2, 4, 6}));
        }

    }

    SECTION("rngPromotingTransform")
    {
        auto rng1 = rngPromotingTransform(f1, f2, Plus{});
        auto rng2 = rngPromotingTransform(f1, scalar(1), Plus{});
        auto rng3 = rngPromotingTransform(scalar(1), f2, Plus{});

        CHECK(MockField<scalar>(rng1) == MockField<scalar>({2, 4, 6}));
        CHECK(MockField<scalar>(rng2) == MockField<scalar>({2, 3, 4}));
        CHECK(MockField<scalar>(rng3) == MockField<scalar>({2, 3, 4}));
    }

    SECTION("returning from function")
    {
        SECTION("No temporary")
        {
            auto func = [](const MockField<scalar>& in){
                return FoamRanges::transform(in, PlusOne{});
            };
            auto rng = func(f1);
            CHECK(MockField<scalar>(rng) == MockField<scalar>({2, 3, 4}));
        }
        SECTION("With temporary")
        {
            auto func = [](const MockField<scalar>& in){
                return FoamRanges::transform(FoamRanges::transform(in, PlusOne{}), PlusOne{});
            };
            auto rng = func(f1);
            CHECK(MockField<scalar>(rng) == MockField<scalar>({3, 4, 5}));
        }
        SECTION("With named temporary")
        {
            auto func = [](const MockField<scalar>& in){
                const auto rng = FoamRanges::transform(in, PlusOne{});
                //return MockField<scalar>(FoamRanges::transform(rng, PlusOne{}));
                return FoamRanges::transform(rng, PlusOne{});
            };
            auto rng = func(f1);
            CHECK(MockField<scalar>(rng) == MockField<scalar>({3, 4, 5}));
        }

    }
}


TEST_CASE("Test-MdRange")
{
    MockFieldField f1 = MockFieldField();
    const MockFieldField f2 = MockFieldField();

    SECTION("mdTransform")
    {
        SECTION("unary")
        {
            auto rng1 = FoamRanges::mdTransform(f1, PlusOne{});
            auto rng2 = FoamRanges::mdTransform(f2, PlusOne{});

            CHECK(MockField<scalar>(rng1[0]) == MockField<scalar>({2, 2, 2}));
            CHECK(MockField<scalar>(rng1[1]) == MockField<scalar>({3, 3, 3, 3}));
            CHECK(MockField<scalar>(rng1[2]) == MockField<scalar>({4, 4, 4, 4, 4}));

            CHECK(MockField<scalar>(rng2[0]) == MockField<scalar>({2, 2, 2}));
            CHECK(MockField<scalar>(rng2[1]) == MockField<scalar>({3, 3, 3, 3}));
            CHECK(MockField<scalar>(rng2[2]) == MockField<scalar>({4, 4, 4, 4, 4}));

        }

        SECTION("binary")
        {
            auto rng1 = FoamRanges::mdTransform(f1, f2, Plus{});
            auto rng2 = FoamRanges::mdTransform(f2, f1, Plus{});

            CHECK(MockField<scalar>(rng1[0]) == MockField<scalar>({2, 2, 2}));
            CHECK(MockField<scalar>(rng1[1]) == MockField<scalar>({4, 4, 4, 4}));
            CHECK(MockField<scalar>(rng1[2]) == MockField<scalar>({6, 6, 6, 6, 6}));

            CHECK(MockField<scalar>(rng2[0]) == MockField<scalar>({2, 2, 2}));
            CHECK(MockField<scalar>(rng2[1]) == MockField<scalar>({4, 4, 4, 4}));
            CHECK(MockField<scalar>(rng2[2]) == MockField<scalar>({6, 6, 6, 6, 6}));

        }


    }

    SECTION("mdrngPromotingTransform")
    {
        auto rng1 = mdrngPromotingTransform(f1, f2, Plus{});
        auto rng2 = mdrngPromotingTransform(scalar(1), f2, Plus{});
        auto rng3 = mdrngPromotingTransform(f1, scalar(1), Plus{});

        CHECK(MockField<scalar>(rng1[0]) == MockField<scalar>({2, 2, 2}));
        CHECK(MockField<scalar>(rng1[1]) == MockField<scalar>({4, 4, 4, 4}));
        CHECK(MockField<scalar>(rng1[2]) == MockField<scalar>({6, 6, 6, 6, 6}));

        CHECK(MockField<scalar>(rng2[0]) == MockField<scalar>({2, 2, 2}));
        CHECK(MockField<scalar>(rng2[1]) == MockField<scalar>({3, 3, 3, 3}));
        CHECK(MockField<scalar>(rng2[2]) == MockField<scalar>({4, 4, 4, 4, 4}));

        CHECK(MockField<scalar>(rng3[0]) == MockField<scalar>({2, 2, 2}));
        CHECK(MockField<scalar>(rng3[1]) == MockField<scalar>({3, 3, 3, 3}));
        CHECK(MockField<scalar>(rng3[2]) == MockField<scalar>({4, 4, 4, 4, 4}));

    }

    SECTION("Returning from function")
    {
        SECTION("No temporary")
        {
            auto func = [](const MockFieldField& in){
                return FoamRanges::mdTransform(in, PlusOne{});
            };
            auto rng = func(f1);
            CHECK(MockField<scalar>(rng[0]) == MockField<scalar>({2, 2, 2}));
            CHECK(MockField<scalar>(rng[1]) == MockField<scalar>({3, 3, 3, 3}));
            CHECK(MockField<scalar>(rng[2]) == MockField<scalar>({4, 4, 4, 4, 4}));
        }

        SECTION("With temporary")
        {
            auto func = [](const MockFieldField& in){
                return FoamRanges::mdTransform(FoamRanges::mdTransform(in, PlusOne{}), PlusOne{});
            };
            auto rng = func(f1);
            CHECK(MockField<scalar>(rng[0]) == MockField<scalar>({3, 3, 3}));
            CHECK(MockField<scalar>(rng[1]) == MockField<scalar>({4, 4, 4, 4}));
            CHECK(MockField<scalar>(rng[2]) == MockField<scalar>({5, 5, 5, 5, 5}));
        }


        SECTION("With named temporary")
        {
            /*
            auto func = [](const MockFieldField& in){
                //This goes out of scope because MdTransformRange stores a reference to the temporary
                const auto rng = FoamRanges::mdTransform(in, PlusOne{});
                return FoamRanges::mdTransform(rng, PlusOne{});
            };
            auto rng = func(f1);
            CHECK(MockField<scalar>(rng[0]) == MockField<scalar>({3, 3, 3}));
            CHECK(MockField<scalar>(rng[1]) == MockField<scalar>({4, 4, 4, 4}));
            CHECK(MockField<scalar>(rng[2]) == MockField<scalar>({5, 5, 5, 5, 5}));
            */
        }

    }



}

TEST_CASE("Test-DimensionedRange")
{
    MockDimField f1 = MockDimField();
    const MockDimField f2 = MockDimField();


    SECTION("dimensionedTransform")
    {
        SECTION("unary")
        {
            auto rng1 = FoamRanges::dimensionedTransform(f1, PlusOne{});
            auto rng2 = FoamRanges::dimensionedTransform(f2, PlusOne{});
            CHECK(rng1.dimensions() == dimless);
            CHECK(rng2.dimensions() == dimless);
            CHECK(rng1.name() == "p");
            CHECK(rng2.name() == "p");
            CHECK(MockField<scalar>(rng1.primitiveField()) == MockField<scalar>({2, 2, 2}));
            CHECK(MockField<scalar>(rng2.primitiveField()) == MockField<scalar>({2, 2, 2}));
        }

        SECTION("binary")
        {
            auto rng1 = FoamRanges::dimensionedTransform(f1, f2, Plus{});
            auto rng2 = FoamRanges::dimensionedTransform(f2, f1, Plus{});
            CHECK(rng1.dimensions() == dimless);
            CHECK(rng2.dimensions() == dimless);
            CHECK(rng1.name() == "(p+p)");
            CHECK(rng2.name() == "(p+p)");
            CHECK(MockField<scalar>(rng1.primitiveField()) == MockField<scalar>({2, 2, 2}));
            CHECK(MockField<scalar>(rng2.primitiveField()) == MockField<scalar>({2, 2, 2}));
        }

    }

    SECTION("dimensioned_promoting_transform")
    {
        auto rng1 = dimrngPromotingTransform(f1, f2, Plus{});
        auto rng2 = dimrngPromotingTransform(f1, scalar(1), Plus{});
        auto rng3 = dimrngPromotingTransform(scalar(1), f2, Plus{});

        CHECK(rng1.dimensions() == dimless);
        CHECK(rng2.dimensions() == dimless);
        CHECK(rng3.dimensions() == dimless);
        CHECK(rng1.name() == "(p+p)");
        CHECK(rng2.name() == "(p+1)");
        CHECK(rng3.name() == "(1+p)");
        CHECK(MockField<scalar>(rng1.primitiveField()) == MockField<scalar>({2, 2, 2}));
        CHECK(MockField<scalar>(rng2.primitiveField()) == MockField<scalar>({2, 2, 2}));
        CHECK(MockField<scalar>(rng3.primitiveField()) == MockField<scalar>({2, 2, 2}));
    }


    SECTION("Returning from function")
    {
        SECTION("No temporary")
        {
            auto func = [](const MockDimField& in){
                return FoamRanges::dimensionedTransform(in, PlusOne{});
            };
            auto rng = func(f1);
            CHECK(rng.dimensions() == dimless);
            CHECK(rng.name() == "p");
            CHECK(MockField<scalar>(rng.primitiveField()) == MockField<scalar>({2, 2, 2}));
        }

        SECTION("With temporary")
        {
            auto func = [](const MockDimField& in){
                return FoamRanges::dimensionedTransform(FoamRanges::dimensionedTransform(in, PlusOne{}), PlusOne{});
            };
            auto rng = func(f1);
            CHECK(rng.dimensions() == dimless);
            CHECK(rng.name() == "p");
            CHECK(MockField<scalar>(rng.primitiveField()) == MockField<scalar>({3, 3, 3}));
        }

        SECTION("With named temporary")
        {

            auto func = [](const MockDimField& in){
                //This wont go out of scope because DimensionedTransformRange stores a copy of the interators
                const auto rng = FoamRanges::dimensionedTransform(in, PlusOne{});
                return FoamRanges::dimensionedTransform(rng, PlusOne{});
            };
            auto rng = func(f1);
            CHECK(rng.dimensions() == dimless);
            CHECK(rng.name() == "p");
            CHECK(MockField<scalar>(rng.primitiveField()) == MockField<scalar>({3, 3, 3}));
        }
    }
}


TEST_CASE("Test-GeometricRange")
{
    MockGeoField f1 = MockGeoField();
    const MockGeoField f2 = MockGeoField();



    SECTION("geometricTransform")
    {
        SECTION("unary")
        {
            auto rng1 = FoamRanges::geometricTransform(f1, PlusOne{});
            auto rng2 = FoamRanges::geometricTransform(f2, PlusOne{});
            CHECK(rng1.dimensions() == dimless);
            CHECK(rng2.dimensions() == dimless);
            CHECK(rng1.name() == "p");
            CHECK(rng2.name() == "p");

            CHECK(MockField<scalar>(rng1.internalField().primitiveField()) == MockField<scalar>({2, 2, 2}));
            CHECK(MockField<scalar>(rng1.boundaryField()[0]) == MockField<scalar>({2, 2, 2}));
            CHECK(MockField<scalar>(rng1.boundaryField()[1]) == MockField<scalar>({3, 3, 3, 3}));
            CHECK(MockField<scalar>(rng1.boundaryField()[2]) == MockField<scalar>({4, 4, 4, 4, 4}));

            CHECK(MockField<scalar>(rng2.internalField().primitiveField()) == MockField<scalar>({2, 2, 2}));
            CHECK(MockField<scalar>(rng2.boundaryField()[0]) == MockField<scalar>({2, 2, 2}));
            CHECK(MockField<scalar>(rng2.boundaryField()[1]) == MockField<scalar>({3, 3, 3, 3}));
            CHECK(MockField<scalar>(rng2.boundaryField()[2]) == MockField<scalar>({4, 4, 4, 4, 4}));

        }

        SECTION("binary")
        {
            auto rng1 = FoamRanges::geometricTransform(f1, f2, Plus{});
            auto rng2 = FoamRanges::geometricTransform(f2, f1, Plus{});
            CHECK(rng1.dimensions() == dimless);
            CHECK(rng2.dimensions() == dimless);
            CHECK(rng1.name() == "(p+p)");
            CHECK(rng2.name() == "(p+p)");

            CHECK(MockField<scalar>(rng1.internalField().primitiveField()) == MockField<scalar>({2, 2, 2}));
            CHECK(MockField<scalar>(rng1.boundaryField()[0]) == MockField<scalar>({2, 2, 2}));
            CHECK(MockField<scalar>(rng1.boundaryField()[1]) == MockField<scalar>({4, 4, 4, 4}));
            CHECK(MockField<scalar>(rng1.boundaryField()[2]) == MockField<scalar>({6, 6, 6, 6, 6}));

            CHECK(MockField<scalar>(rng2.internalField().primitiveField()) == MockField<scalar>({2, 2, 2}));
            CHECK(MockField<scalar>(rng2.boundaryField()[0]) == MockField<scalar>({2, 2, 2}));
            CHECK(MockField<scalar>(rng2.boundaryField()[1]) == MockField<scalar>({4, 4, 4, 4}));
            CHECK(MockField<scalar>(rng2.boundaryField()[2]) == MockField<scalar>({6, 6, 6, 6, 6}));
        }
    }

    SECTION("georngPromotingTransform")
    {
        auto rng1 = georngPromotingTransform(f1, f2, Plus{});
        auto rng2 = georngPromotingTransform(f1, scalar(1), Plus{});
        auto rng3 = georngPromotingTransform(scalar(1), f2, Plus{});

        CHECK(rng1.dimensions() == dimless);
        CHECK(rng2.dimensions() == dimless);
        CHECK(rng3.dimensions() == dimless);
        CHECK(rng1.name() == "(p+p)");
        CHECK(rng2.name() == "(p+1)");
        CHECK(rng3.name() == "(1+p)");

        CHECK(MockField<scalar>(rng1.internalField().primitiveField()) == MockField<scalar>({2, 2, 2}));
        CHECK(MockField<scalar>(rng1.boundaryField()[0]) == MockField<scalar>({2, 2, 2}));
        CHECK(MockField<scalar>(rng1.boundaryField()[1]) == MockField<scalar>({4, 4, 4, 4}));
        CHECK(MockField<scalar>(rng1.boundaryField()[2]) == MockField<scalar>({6, 6, 6, 6, 6}));

        CHECK(MockField<scalar>(rng2.internalField().primitiveField()) == MockField<scalar>({2, 2, 2}));
        CHECK(MockField<scalar>(rng2.boundaryField()[0]) == MockField<scalar>({2, 2, 2}));
        CHECK(MockField<scalar>(rng2.boundaryField()[1]) == MockField<scalar>({3, 3, 3, 3}));
        CHECK(MockField<scalar>(rng2.boundaryField()[2]) == MockField<scalar>({4, 4, 4, 4, 4}));

        CHECK(MockField<scalar>(rng3.internalField().primitiveField()) == MockField<scalar>({2, 2, 2}));
        CHECK(MockField<scalar>(rng3.boundaryField()[0]) == MockField<scalar>({2, 2, 2}));
        CHECK(MockField<scalar>(rng3.boundaryField()[1]) == MockField<scalar>({3, 3, 3, 3}));
        CHECK(MockField<scalar>(rng3.boundaryField()[2]) == MockField<scalar>({4, 4, 4, 4, 4}));
    }

    SECTION("Returning from function")
    {
        SECTION("No temporary")
        {
            auto func = [](const MockGeoField& in){
                return FoamRanges::geometricTransform(in, PlusOne{});
            };
            auto rng = func(f1);
            CHECK(rng.dimensions() == dimless);
            CHECK(rng.name() == "p");
            CHECK(MockField<scalar>(rng.internalField().primitiveField()) == MockField<scalar>({2, 2, 2}));
            CHECK(MockField<scalar>(rng.boundaryField()[0]) == MockField<scalar>({2, 2, 2}));
            CHECK(MockField<scalar>(rng.boundaryField()[1]) == MockField<scalar>({3, 3, 3, 3}));
            CHECK(MockField<scalar>(rng.boundaryField()[2]) == MockField<scalar>({4, 4, 4, 4, 4}));
        }

        SECTION("With temporary")
        {
            auto func = [](const MockGeoField& in){
                return FoamRanges::geometricTransform(FoamRanges::geometricTransform(in, PlusOne{}), PlusOne{});
            };
            auto rng = func(f1);
            CHECK(rng.dimensions() == dimless);
            CHECK(rng.name() == "p");
            CHECK(MockField<scalar>(rng.internalField().primitiveField()) == MockField<scalar>({3, 3, 3}));
            CHECK(MockField<scalar>(rng.boundaryField()[0]) == MockField<scalar>({3, 3, 3}));
            CHECK(MockField<scalar>(rng.boundaryField()[1]) == MockField<scalar>({4, 4, 4, 4}));
            CHECK(MockField<scalar>(rng.boundaryField()[2]) == MockField<scalar>({5, 5, 5, 5, 5}));
        }

        SECTION("With named temporary")
        {
            auto func = [](const MockGeoField& in){
                //No idea why this works.
                //The temporary should go out of scope because the
                //MdTransformRange of a GeometricTransformRange stores a reference to the temporary.
                const auto rng = FoamRanges::geometricTransform(in, PlusOne{});
                return FoamRanges::geometricTransform(rng, PlusOne{});
            };
            auto rng = func(f1);
            CHECK(rng.dimensions() == dimless);
            CHECK(rng.name() == "p");
            CHECK(MockField<scalar>(rng.primitiveField()) == MockField<scalar>({3, 3, 3}));
            CHECK(MockField<scalar>(rng.internalField().primitiveField()) == MockField<scalar>({3, 3, 3}));
            CHECK(MockField<scalar>(rng.boundaryField()[0]) == MockField<scalar>({3, 3, 3}));
            CHECK(MockField<scalar>(rng.boundaryField()[1]) == MockField<scalar>({4, 4, 4, 4}));
            CHECK(MockField<scalar>(rng.boundaryField()[2]) == MockField<scalar>({5, 5, 5, 5, 5}));

        }
    }

}





