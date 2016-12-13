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

#include "xdump.hpp"
#include <iomanip>
#include <sstream>

std::string pMR::xdump(void const *data, std::size_t sizeByte)
{
    char const *const start = static_cast<char const *>(data);
    char const *const end = start + sizeByte;
    const char *line = start;

    std::stringstream dump;
    while(line != end)
    {
        auto length = end - line;
        decltype(length) maxLength = 16;
        length = std::min(length, maxLength);

        dump << std::setw(4);
        dump << std::setfill('0');
        dump << std::hex << line - start << ": ";

        for(char const *c = line; c != line + length; ++c)
        {
            dump << std::setw(2);
            dump << std::setfill('0');
            dump << std::hex << std::uppercase
                 << static_cast<int>(static_cast<unsigned char>(*c)) << ' ';
        }

        dump << std::string((16 - length) * 3, ' ');

        for(char const *c = line; c != line + length; ++c)
        {
            dump << (*c < 32 ? '.' : *c);
        }

        dump << std::endl;
        line += length;
    }

    return dump.str();
}
