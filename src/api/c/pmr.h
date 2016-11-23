//  Copyright 2016 Peter Georg
//
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
//  Unless required by applicable law or agreed to in writing, software
//  distributed under the License is distributed on an "AS IS" BASIS,
//  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//  See the License for the specific language governing permissions and
//  limitations under the License.

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
void pMR_PostSendPartial(pMR_SendWindow*, uint32_t sizeByte);

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
