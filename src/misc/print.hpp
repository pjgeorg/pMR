#ifndef pMR_MISC_PRINT_H
#define pMR_MISC_PRINT_H

#include <iostream>
#include "string.hpp"

namespace pMR
{
    template<typename ...Types>
    void print(Types const &...args);
}

template<typename ...Types>
void pMR::print(Types const &...args)
{
    std::string str = toString(args...) + "\n";
    std::cout << str;
}
#endif // pMR_MISC_PRINT_H
