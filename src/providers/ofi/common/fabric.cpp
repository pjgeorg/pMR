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

#include "fabric.hpp"
#include <stdexcept>

pMR::OFI::Fabric::Fabric(Info &info)
{
    if(fi_fabric(info.getFabric(), &mFabric, &mContext))
    {
        throw std::runtime_error("pMR: Unable to open fabric.");
    }
}

pMR::OFI::Fabric::~Fabric()
{
    if(mFabric)
    {
        fi_close(&mFabric->fid);
    }
}

fid_fabric *pMR::OFI::Fabric::get()
{
    return mFabric;
}

fid_fabric const *pMR::OFI::Fabric::get() const
{
    return mFabric;
}
