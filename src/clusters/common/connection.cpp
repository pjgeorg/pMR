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

#include "connection.hpp"

pMR::Connection::Connection(Target const &target)
{
    connect(target);
}

#ifdef pMR_PROVIDER_CMA
#include "../../providers/cma/connection.hpp"

void pMR::Connection::connectCMA(Target const &target)
{
    mProvider = Provider::cma;
    mCMA = std::make_shared<cma::Connection>(target);
}
#endif // pMR_PROVIDER_CMA

#ifdef pMR_PROVIDER_MPI
#include "../../providers/mpi/connection.hpp"

void pMR::Connection::connectMPI(Target const &target)
{
    mProvider = Provider::mpi;
    mMPI = std::make_shared<mpi::Connection>(target);
}
#endif // pMR_PROVIDER_MPI

#ifdef pMR_PROVIDER_NULL
#include "../../providers/null/connection.hpp"

void pMR::Connection::connectNull(Target const &target)
{
    mProvider = Provider::null;
    mNull = std::make_shared<null::Connection>(target);
}
#endif // pMR_PROVIDER_NULL

#ifdef pMR_PROVIDER_OFI
#include "../../providers/ofi/connection.hpp"

void pMR::Connection::connectOFI(Target const &target, ofi::Info const &info)
{
    mProvider = Provider::ofi;
    mOFI = std::make_shared<ofi::Connection>(target, info);
}
#endif // pMR_PROVIDER_OFI

#ifdef pMR_PROVIDER_PSM2
#include "../../providers/psm2/connection.hpp"

void pMR::Connection::connectPSM2(Target const &target)
{
    mProvider = Provider::psm2;
    mPSM2 = std::make_shared<psm2::Connection>(target);
}
#endif // pMR_PROVIDER_PSM2

#ifdef pMR_PROVIDER_SCIF
#include "../../providers/scif/connection.hpp"

void pMR::Connection::connectSCIF(Target const &target)
{
    mProvider = Provider::scif;
    mSCIF = std::make_shared<scif::Connection>(target);
}
#endif // pMR_PROVIDER_SCIF

#ifdef pMR_PROVIDER_SELF
#include "../../providers/self/connection.hpp"

void pMR::Connection::connectSelf(Target const &target)
{
    mProvider = Provider::self;
    mSelf = std::make_shared<self::Connection>(target);
}
#endif // pMR_PROVIDER_SELF

#ifdef pMR_PROVIDER_VERBS
#include "../../providers/verbs/connection.hpp"

void pMR::Connection::connectVerbs(Target const &target,
    verbs::Device const &device, std::uint8_t const portNumber)
{
    mProvider = Provider::verbs;
    mVerbs = std::make_shared<verbs::Connection>(target, device, portNumber);
}
#endif // pMR_PROVIDER_VERBS
