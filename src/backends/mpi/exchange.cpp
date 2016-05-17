#include "../backend.hpp"
#include <stdexcept>
extern "C"
{
#include <mpi.h>
}
#include "target.hpp"
#include "threadsupport.hpp"
#include "../../misc/singleton.hpp"
#include "../../threads/thread.hpp"

void pMR::backend::exchange(Target const &target,
        void *sendBuffer, void *recvBuffer, std::uint32_t const sizeByte)
{
    if(Singleton<ThreadSupport>::Instance().multiple() || !thread::isMT())
    {
        if(MPI_Sendrecv(sendBuffer, sizeByte, MPI_BYTE,
                target.getTargetRank(),
                target.getUniqueSendID(),
                recvBuffer, sizeByte, MPI_BYTE,
                target.getTargetRank(),
                target.getUniqueRecvID(), 
                target.getMPICommunicator(),
                MPI_STATUS_IGNORE) != MPI_SUCCESS)
        {
            throw std::runtime_error(
                    "pMR: Unable to exchange connection data.");
        }
    }
    else
    {
        if(Singleton<ThreadSupport>::Instance().serialized())
        {
            MPI_Request sendRequest;
            MPI_Request recvRequest;
            {
                thread::ScopedLock scopedLock;
                if(MPI_Isend(sendBuffer, sizeByte, MPI_BYTE,
                        target.getTargetRank(),
                        target.getUniqueSendID(),
                        target.getMPICommunicator(),
                        &sendRequest) != MPI_SUCCESS)
                {
                    throw std::runtime_error(
                            "pMR: Unable to exchange connection data");
                }
                if(MPI_Irecv(recvBuffer, sizeByte, MPI_BYTE,
                        target.getTargetRank(),
                        target.getUniqueRecvID(),
                        target.getMPICommunicator(),
                        &recvRequest) != MPI_SUCCESS)
                {
                    throw std::runtime_error(
                            "pMR: Unable to exchange connection data");
                }
            }
    
            int flag = false;
            while(!flag)
            {
                thread::ScopedLock scopedLock;
                if(MPI_Test(&sendRequest, &flag, MPI_STATUS_IGNORE)
                        != MPI_SUCCESS)
                {
                    throw std::runtime_error(
                            "pMR: Unable to exchange connection data");
                }
            }
            flag = false;
            while(!flag)
            {
                thread::ScopedLock scopedLock;
                if(MPI_Test(&recvRequest, &flag, MPI_STATUS_IGNORE)
                        != MPI_SUCCESS)
                {
                    throw std::runtime_error(
                            "pMR: Unable to exchange connection data");
                }
            }
        }
        else
        {
            throw std::runtime_error("pMR: Require at least "
                    "MPI_THREAD_SERIALIZED Thread support");
        }
    }
}
