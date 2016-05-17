#ifndef pMR_MISC_TYPE_H
#define pMR_MISC_TYPE_H

#include <iterator>

namespace pMR
{
    template<class Iterator>
    constexpr bool isRandomAccessIterator();
}

template<class Iterator>
constexpr bool pMR::isRandomAccessIterator()
{
    return std::is_base_of<std::random_access_iterator_tag,
           typename std::iterator_traits<Iterator>::iterator_category>::value;
}
#endif // pMR_MISC_TYPE_H
