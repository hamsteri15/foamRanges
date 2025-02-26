/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
    \\  /    A nd           | Version:  10
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

Description
    Types that allow for mocking of different OpenFOAM field types.

SourceFiles
    mockFields.C

\*---------------------------------------------------------------------------*/

#pragma once

#include "newField.H"
#include "dimensionedType.H"

namespace Foam
{

struct MockMesh
{
    label size() const { return 3; }
};

template<class T>
using MockField = NewField<T>;

struct MockDimField
{
    using iterator = typename MockField<scalar>::iterator;
    using GeoMeshType = MockMesh;
    static constexpr bool is_foam_field = false;
    static constexpr bool is_foam_field_field = false;
    static constexpr bool is_foam_dimensioned_field = true;
    static constexpr bool is_foam_geometric_field = false;

    MockDimField() {}

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

    label size() const { return m_mesh.size(); }

    const dimensionSet& dimensions() const { return m_dimensions; }
    GeoMeshType mesh() const { return m_mesh; }
    const word& name() const { return m_name; }
    const MockField<scalar>& primitiveField() const { return m_primitive; }

    dimensionSet m_dimensions = Foam::dimless;
    word m_name = "p";
    MockMesh m_mesh = MockMesh();
    MockField<scalar> m_primitive = MockField<scalar>(m_mesh.size(), 1);
};

struct MockFieldField : public PtrList<MockField<scalar>>
{
    using base_type = PtrList<MockField<scalar>>;

    static constexpr bool is_foam_field = false;
    static constexpr bool is_foam_field_field = true;
    static constexpr bool is_foam_dimensioned_field = false;
    static constexpr bool is_foam_geometric_field = false;

    MockFieldField() : base_type(3)
    {
        this->set(0, new MockField<scalar>({1, 1, 1}));
        this->set(1, new MockField<scalar>({2, 2, 2, 2}));
        this->set(2, new MockField<scalar>({3, 3, 3, 3, 3}));
    }

    MockFieldField(const MockFieldField& other) : base_type(3)
    {
        throw std::logic_error("NO COPIES SHOULD BE MADE");
    }

    MockFieldField(MockFieldField&& other)
    {
        throw std::logic_error("NO MOVES SHOULD BE MADE");
    }

    MockFieldField& operator=(const MockFieldField& other)
    {
        throw std::logic_error("NO COPIES SHOULD BE MADE");
    }

    MockFieldField& operator=(MockFieldField&& other)
    {
        throw std::logic_error("NO MOVES SHOULD BE MADE");
    }
};

struct MockGeoField : public MockDimField
{
    static constexpr bool is_foam_field = false;
    static constexpr bool is_foam_field_field = false;
    static constexpr bool is_foam_dimensioned_field = false;
    static constexpr bool is_foam_geometric_field = true;

    MockGeoField() {}

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

} // namespace Foam