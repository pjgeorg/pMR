#include "../backend.hpp"

void pMR::backend::sync(Target const &target)
{
    exchange(target, nullptr, nullptr, 0);
}
