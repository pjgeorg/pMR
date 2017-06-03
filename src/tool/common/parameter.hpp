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

#ifndef pMR_TOOL_COMMON_PARAMETER_H
#define pMR_TOOL_COMMON_PARAMETER_H

#include <algorithm>
#include <array>
#include <sstream>
#include <string>
#include <vector>

bool parameterExists(
    char *const *begin, char *const *end, std::string const &parameter);

template<typename T>
void parameterOption(
    char **begin, char **end, std::string const &parameter, T &value)
{
    char **input = std::find(begin, end, parameter);

    if(input != end)
    {
        if(++input != end)
        {
            std::istringstream stream(*input);
            stream >> value;
        }
        else
        {
            throw std::invalid_argument(parameter);
        }
    }
}
template<typename T>
void parameterOption(char **begin, char **end, std::string const &parameter,
    std::vector<T> &option)
{
    char **input = std::find(begin, end, parameter);

    if(input != end)
    {
        for(auto &value : option)
        {
            if(++input != end)
            {
                for(auto &character : std::string(*input))
                {
                    if(!isdigit(character))
                    {
                        throw std::invalid_argument(parameter);
                    }
                }
                std::istringstream stream(*input);
                stream >> value;
            }
            else
            {
                throw std::invalid_argument(parameter);
            }
        }
    }
}
#endif // pMR_TOOL_COMMON_PARAMETER_H
