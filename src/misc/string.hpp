#ifndef pMR_MISC_STRING_H
#define pMR_MISC_STRING_H

#include <string>
#include <sstream>

namespace pMR
{
    std::string toString();

    template<typename ...Types>
    std::string toString(double const &arg, Types const &...args);

    template<typename T, typename ...Types>
    std::string toString(T const &arg, Types const &...args);

    template<std::size_t N, typename ...Types>
    std::string toString(char const (&arg)[N], Types const &...args);
}

template<typename ...Types>
std::string pMR::toString(double const &arg, Types const &...args)
{
    std::string str;
    std::ostringstream oss;
    oss << std::scientific << arg;
    str += oss.str();
    str += " ";
    str += toString(args...);
    return str;
}

template<typename T, typename ...Types>
std::string pMR::toString(T const &arg, Types const &...args)
{
    std::string str;
    std::ostringstream oss;
    oss << arg;
    str += oss.str();
    str += " ";
    str += toString(args...);
    return str;
}

template<std::size_t N, typename ...Types>
std::string pMR::toString(char const (&arg)[N], Types const &...args)
{
    std::string str;
    str += arg;
    str += " ";
    str += toString(args...);
    return str;
}
#endif // pMR_MISC_STRING_H
