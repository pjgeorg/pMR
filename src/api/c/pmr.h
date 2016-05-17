#ifndef pMR_API_C_PMR_H
#define pMR_API_C_PMR_H

#include <mpi.h>
#include <stdint.h>

struct pMR_Connection;
typedef struct pMR_Connection pMR_Connection;
pMR_Connection* pMR_CreateConnection(MPI_Comm communicator, int targetRank,
        int uniqueSendID, int uniqueRecvID);
void pMR_DestroyConnection(pMR_Connection*);

struct pMR_SendWindow;
typedef struct pMR_SendWindow pMR_SendWindow;
pMR_SendWindow* pMR_CreateSendWindow(pMR_Connection*,
        void *buffer, uint32_t sizeByte);
void pMR_DestroySendWindow(pMR_SendWindow*);
int pMR_IsSameSendWindow(pMR_SendWindow*, void *buffer, uint32_t sizeByte);
void pMR_InitSend(pMR_SendWindow*);
void pMR_PostSend(pMR_SendWindow*);
void pMR_WaitSend(pMR_SendWindow*);

struct pMR_RecvWindow;
typedef struct pMR_RecvWindow pMR_RecvWindow;
pMR_RecvWindow* pMR_CreateRecvWindow(pMR_Connection*,
        void *buffer, uint32_t sizeByte);
void pMR_DestroyRecvWindow(pMR_RecvWindow*);
int pMR_IsSameRecvWindow(pMR_RecvWindow*, void *buffer, uint32_t sizeByte);
void pMR_InitRecv(pMR_RecvWindow*);
void pMR_PostRecv(pMR_RecvWindow*);
void pMR_WaitRecv(pMR_RecvWindow*);
#endif // pMR_API_C_PMR_H
