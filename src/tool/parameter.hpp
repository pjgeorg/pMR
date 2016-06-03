#ifndef pMR_TOOL_PARAMETER_H
#define pMR_TOOL_PARAMETER_H

#include <string>
#include <array>
#include <algorithm>
#include <sstream>
#include "usage.hpp"

bool parameterExists(char *const *begin, char *const *end,
        std::string const &parameter);

template<typename T>
void parameterOption(char **begin, char **end,
        std::string const &parameter, T &value)
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
            printUsage();
        }
    }
}
template<typename T>
void parameterOption(char **begin, char **end,
        std::string const &parameter, std::vector<T> &option)
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
                        printUsage();
                    }
                }
                std::istringstream stream(*input);
                stream >> value;
            }
            else
            {
                printUsage();
            }
        }
    }
}
#endif // pMR_TOOL_PARAMETER_H
