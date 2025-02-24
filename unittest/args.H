#pragma once

#include <mpi.h>
//#include <Pstream.H>
#include "argList.H"


extern Foam::argList* argsPtr;

inline Foam::argList& getFoamArgs()
{
    return *argsPtr;
}

inline void setFoamArgs(int argc, char* argv[])
{
    argsPtr = new Foam::argList(argc,argv);
}

inline void clearFoamArgs()
{
    delete argsPtr;
}

static inline int mpi_initialized()
{
    int ret;
    MPI_Initialized(&ret);
    return ret;
}


static inline void mpi_barrier()
{
    if (mpi_initialized()){
        MPI_Barrier(MPI_COMM_WORLD);
    }
}
