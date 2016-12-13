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

#ifdef NULL_WARN
#include "../../misc/print.hpp"
#endif // NULL_WARN

pMR::null::Connection::Connection(Target const &target)
{
#ifdef NULL_WARN
    print("pMR: Using provider null.");
#endif // NULL_WARN
}
