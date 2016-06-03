#include "parameter.hpp"

bool parameterExists(char *const *begin, char *const *end,
        std::string const &parameter)
{
    return std::find(begin, end, parameter) != end;
}
