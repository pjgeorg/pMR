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

#ifndef pMR_PROVIDERS_OFI_SENDMEMORYWINDOW_H
#define pMR_PROVIDERS_OFI_SENDMEMORYWINDOW_H

#ifdef OFI_EP_MSG
#include "msg/sendmemorywindow.hpp"
#elif defined OFI_EP_RDM
#include "rdm/sendmemorywindow.hpp"
#else
#error "Unknown endpoint."
#endif // OFI_EP

#endif // pMR_PROVIDERS_OFI_SENDMEMORYWINDOW_H
