#define CATCH_CONFIG_ENABLE_BENCHMARKING
#include "catch.H"
#include "args.H"
#include "field_reader.H"
#include "geomesh_access.H"
#include "approx_equal.H"
#include "mock_fields.H"

#include "operations.H"

#include "dimensionedType.H"
#include "promoting_transforms.hpp"


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

    SECTION("rng_promoting_transform")
    {
        auto rng1 = detail::rng_promoting_transform(f1, f2, Plus{});
        auto rng2 = detail::rng_promoting_transform(f1, scalar(1), Plus{});
        auto rng3 = detail::rng_promoting_transform(scalar(1), f2, Plus{});

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

    SECTION("md_transform")
    {
        SECTION("unary")
        {
            auto rng1 = FoamRanges::md_transform(f1, PlusOne{});
            auto rng2 = FoamRanges::md_transform(f2, PlusOne{});

            CHECK(MockField<scalar>(rng1[0]) == MockField<scalar>({2, 2, 2}));
            CHECK(MockField<scalar>(rng1[1]) == MockField<scalar>({3, 3, 3, 3}));
            CHECK(MockField<scalar>(rng1[2]) == MockField<scalar>({4, 4, 4, 4, 4}));

            CHECK(MockField<scalar>(rng2[0]) == MockField<scalar>({2, 2, 2}));
            CHECK(MockField<scalar>(rng2[1]) == MockField<scalar>({3, 3, 3, 3}));
            CHECK(MockField<scalar>(rng2[2]) == MockField<scalar>({4, 4, 4, 4, 4}));

        }

        SECTION("binary")
        {
            auto rng1 = FoamRanges::md_transform(f1, f2, Plus{});
            auto rng2 = FoamRanges::md_transform(f2, f1, Plus{});

            CHECK(MockField<scalar>(rng1[0]) == MockField<scalar>({2, 2, 2}));
            CHECK(MockField<scalar>(rng1[1]) == MockField<scalar>({4, 4, 4, 4}));
            CHECK(MockField<scalar>(rng1[2]) == MockField<scalar>({6, 6, 6, 6, 6}));

            CHECK(MockField<scalar>(rng2[0]) == MockField<scalar>({2, 2, 2}));
            CHECK(MockField<scalar>(rng2[1]) == MockField<scalar>({4, 4, 4, 4}));
            CHECK(MockField<scalar>(rng2[2]) == MockField<scalar>({6, 6, 6, 6, 6}));

        }


    }

    SECTION("md_promoting_transform")
    {
        auto rng1 = detail::md_promoting_transform(f1, f2, Plus{});
        auto rng2 = detail::md_promoting_transform(scalar(1), f2, Plus{});
        auto rng3 = detail::md_promoting_transform(f1, scalar(1), Plus{});

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
                return FoamRanges::md_transform(in, PlusOne{});
            };
            auto rng = func(f1);
            CHECK(MockField<scalar>(rng[0]) == MockField<scalar>({2, 2, 2}));
            CHECK(MockField<scalar>(rng[1]) == MockField<scalar>({3, 3, 3, 3}));
            CHECK(MockField<scalar>(rng[2]) == MockField<scalar>({4, 4, 4, 4, 4}));
        }

        SECTION("With temporary")
        {
            auto func = [](const MockFieldField& in){
                return FoamRanges::md_transform(FoamRanges::md_transform(in, PlusOne{}), PlusOne{});
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
                const auto rng = FoamRanges::md_transform(in, PlusOne{});
                return FoamRanges::md_transform(rng, PlusOne{});
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


    SECTION("dimensioned_transform")
    {
        SECTION("unary")
        {
            auto rng1 = FoamRanges::dimensioned_transform(f1, PlusOne{});
            auto rng2 = FoamRanges::dimensioned_transform(f2, PlusOne{});
            CHECK(rng1.dimensions() == dimless);
            CHECK(rng2.dimensions() == dimless);
            CHECK(rng1.name() == "p");
            CHECK(rng2.name() == "p");
            CHECK(MockField<scalar>(rng1.primitiveField()) == MockField<scalar>({2, 2, 2}));
            CHECK(MockField<scalar>(rng2.primitiveField()) == MockField<scalar>({2, 2, 2}));
        }

        SECTION("binary")
        {
            auto rng1 = FoamRanges::dimensioned_transform(f1, f2, Plus{});
            auto rng2 = FoamRanges::dimensioned_transform(f2, f1, Plus{});
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
        auto rng1 = detail::dimrng_promoting_transform(f1, f2, Plus{});
        auto rng2 = detail::dimrng_promoting_transform(f1, scalar(1), Plus{});
        auto rng3 = detail::dimrng_promoting_transform(scalar(1), f2, Plus{});

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
                return FoamRanges::dimensioned_transform(in, PlusOne{});
            };
            auto rng = func(f1);
            CHECK(rng.dimensions() == dimless);
            CHECK(rng.name() == "p");
            CHECK(MockField<scalar>(rng.primitiveField()) == MockField<scalar>({2, 2, 2}));
        }

        SECTION("With temporary")
        {
            auto func = [](const MockDimField& in){
                return FoamRanges::dimensioned_transform(FoamRanges::dimensioned_transform(in, PlusOne{}), PlusOne{});
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
                const auto rng = FoamRanges::dimensioned_transform(in, PlusOne{});
                return FoamRanges::dimensioned_transform(rng, PlusOne{});
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



    SECTION("geometric_transform")
    {
        SECTION("unary")
        {
            auto rng1 = FoamRanges::geometric_transform(f1, PlusOne{});
            auto rng2 = FoamRanges::geometric_transform(f2, PlusOne{});
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
            auto rng1 = FoamRanges::geometric_transform(f1, f2, Plus{});
            auto rng2 = FoamRanges::geometric_transform(f2, f1, Plus{});
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

    SECTION("georng_promoting_transform")
    {
        auto rng1 = detail::georng_promoting_transform(f1, f2, Plus{});
        auto rng2 = detail::georng_promoting_transform(f1, scalar(1), Plus{});
        auto rng3 = detail::georng_promoting_transform(scalar(1), f2, Plus{});

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
                return FoamRanges::geometric_transform(in, PlusOne{});
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
                return FoamRanges::geometric_transform(FoamRanges::geometric_transform(in, PlusOne{}), PlusOne{});
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
                const auto rng = FoamRanges::geometric_transform(in, PlusOne{});
                return FoamRanges::geometric_transform(rng, PlusOne{});
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





