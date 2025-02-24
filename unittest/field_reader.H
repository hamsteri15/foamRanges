#pragma once

#include <filesystem>
#include "Time.H"
#include "volFields.H"
#include "fvMesh.H"
#include "args.H"
#include "fileName.H"




namespace Foam{


static Time makeTime(const argList& args)
{
    mpi_barrier();
    return Time("controlDict", args);
}

static fvMesh makeMesh(const Time& runTime)
{
    mpi_barrier();
    return fvMesh
    (
        IOobject
        (
            Foam::fvMesh::defaultRegion,
            runTime.name(),
            runTime,
            Foam::IOobject::MUST_READ
        ),
        false
    );

}




struct FieldReader{


    FieldReader(const argList& args) :
    runTime_(makeTime(args)),
    mesh_(makeMesh(runTime_))
    {
        mpi_barrier();
    }


    const fvMesh& mesh() const
    {
        return mesh_;
    }


    const Time& runTime() const {return runTime_;}


    auto read_scalarField(word name) const
    {
        volScalarField ret
        (
            IOobject
            (
                name,
                runTime().name(),
                mesh(),
                IOobject::MUST_READ,
                IOobject::NO_WRITE
            ),
            mesh()
        );
        mpi_barrier();
        //NOTE! Can not correct boundary conditions here because fields may have
        //field dependencies which are not read yet
        //ret.correctBoundaryConditions();
        return ret;
    }

    auto read_vectorField(word name) const
    {
        volVectorField ret
        (
            IOobject
            (
                name,
                runTime().name(),
                mesh(),
                IOobject::MUST_READ,
                IOobject::NO_WRITE
            ),
            mesh()
        );
        //NOTE! Can not correct boundary conditions here because fields may have
        //field dependencies which are not read yet
        //ret.correctBoundaryConditions();
        mpi_barrier();
        return ret;
    }








private:

    Time runTime_;
    fvMesh mesh_;


};





}

