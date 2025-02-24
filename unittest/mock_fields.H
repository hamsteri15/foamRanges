#pragma once

#include "myField.H"

#include "dimensionedType.H"


namespace Foam{

struct MockMesh{
    label size() const {return 3;}
};

template<class T>
using MockField = NewField<T>;

struct MockDimField{

    using iterator = typename MockField<scalar>::iterator;
    using GeoMeshType = MockMesh;
    static constexpr bool is_foam_field = false;
    static constexpr bool is_foam_field_field = false;
    static constexpr bool is_foam_dimensioned_field = true;
    static constexpr bool is_foam_geometric_field = false;

    MockDimField(){}

    MockDimField(const MockDimField& other)
    {
        throw std::logic_error("NO COPIES SHOULD BE MADE");
    }

    MockDimField(const MockDimField&& other)
    {
        throw std::logic_error("NO MOVES SHOULD BE MADE");
    }

    MockDimField& operator=(const MockDimField& other)
    {
        throw std::logic_error("NO COPIES SHOULD BE MADE");
    }

    MockDimField& operator=(MockDimField&& other)
    {
        throw std::logic_error("NO MOVES SHOULD BE MADE");
    }

    //Not sure if we want this or simply always have f.prmitiveField().size()
    label size() const {return m_mesh.size();}

    const dimensionSet& dimensions() const { return m_dimensions; }
    GeoMeshType mesh() const { return m_mesh; }
    const word& name() const { return m_name; }
    const MockField<scalar>& primitiveField() const { return m_primitive; }



    dimensionSet m_dimensions = Foam::dimless;
    word m_name = "p";
    MockMesh m_mesh = MockMesh();
    MockField<scalar> m_primitive = MockField<scalar>(m_mesh.size(), 1);

};

struct MockFieldField : public PtrList<MockField<scalar>>{


    using base_type = PtrList<MockField<scalar>>;

    static constexpr bool is_foam_field = false;
    static constexpr bool is_foam_field_field = true;
    static constexpr bool is_foam_dimensioned_field = false;
    static constexpr bool is_foam_geometric_field = false;

    MockFieldField() : base_type(3){
        this->set(0, new MockField<scalar>({1, 1, 1}));
        this->set(1, new MockField<scalar>({2, 2, 2, 2}));
        this->set(2, new MockField<scalar>({3, 3, 3, 3, 3}));
    }

    MockFieldField(const MockFieldField& other) : base_type(3){
        throw std::logic_error("NO COPIES SHOULD BE MADE");
    }

    MockFieldField(MockFieldField&& other){
        throw std::logic_error("NO MOVES SHOULD BE MADE");
    }

    MockFieldField& operator=(const MockFieldField& other){
        throw std::logic_error("NO COPIES SHOULD BE MADE");
    }
    MockFieldField& operator=(MockFieldField&& other){
        throw std::logic_error("NO MOVES SHOULD BE MADE");
    }

    //PtrList<MockField<scalar>> m_data;

};

struct MockGeoField : public MockDimField{

    static constexpr bool is_foam_field             = false;
    static constexpr bool is_foam_field_field       = false;
    static constexpr bool is_foam_dimensioned_field = false;
    static constexpr bool is_foam_geometric_field   = true;

    MockGeoField(){}

    MockGeoField(const MockGeoField& other)
    {
        throw std::logic_error("NO COPIES SHOULD BE MADE");
    }

    MockGeoField operator=(const MockGeoField& other)
    {
        throw std::logic_error("NO COPIES SHOULD BE MADE");
    }

    MockGeoField(const MockGeoField&& other)
    {
        throw std::logic_error("NO MOVES SHOULD BE MADE");
    }

    MockGeoField operator=(const MockGeoField&& other)
    {
        throw std::logic_error("NO MOVES SHOULD BE MADE");
    }

    const MockDimField& internalField() const { return *this; }
    const MockFieldField& boundaryField() const { return m_boundary; }

    MockFieldField m_boundary = MockFieldField();
};


}