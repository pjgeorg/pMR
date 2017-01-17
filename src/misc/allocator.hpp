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

#ifndef pMR_MISC_ALLOCATOR_H
#define pMR_MISC_ALLOCATOR_H

#include <cstdint>
#include <limits>
#include <memory>
#include <stdexcept>
#include "config.hpp"

namespace pMR
{
    template<typename T, std::size_t Alignment = alignment>
    class AlignedAllocator
    {
    public:
        using value_type = T;
        using reference = T &;
        using const_reference = T const &;
        using pointer = T *;
        using const_pointer = T const *;
        using size_type = std::size_t;
        using difference_type = std::ptrdiff_t;

        template<class U>
        struct rebind
        {
            using other = AlignedAllocator<U, Alignment>;
        };

        AlignedAllocator() = default;
        AlignedAllocator(AlignedAllocator const &) = default;
        AlignedAllocator(AlignedAllocator &&) = default;
        AlignedAllocator &operator=(AlignedAllocator const &) = delete;
        AlignedAllocator &operator=(AlignedAllocator &&) = delete;
        ~AlignedAllocator() = default;

        template<typename U>
        AlignedAllocator(AlignedAllocator<U, Alignment> const &)
        {
        }

        pointer address(reference r) const
        {
            return &r;
        }

        const_pointer address(const_reference r) const
        {
            return &r;
        }

        constexpr size_type max_size() const
        {
            return std::numeric_limits<size_type>::max() / sizeof(T);
        }

        bool operator!=(AlignedAllocator const &rhs) const
        {
            return !(*this == rhs);
        }

        bool operator==(AlignedAllocator const &) const
        {
            return true;
        }

        void construct(pointer const p, const_reference r) const
        {
            new(p) value_type(r);
        }

        void destroy(pointer const p) const
        {
            p->~value_type();
        }

        pointer allocate(size_type const n) const
        {
            if(n == 0)
            {
                return nullptr;
            }

            if(n > max_size())
            {
                throw std::length_error(
                    "AlignedAllocator<T>::allocate() - Integer Overflow.");
            }

            void *p;
            if(posix_memalign(&p, Alignment, n * sizeof(T)))
            {
                throw std::bad_alloc();
            }

            return static_cast<pointer>(p);
        }

        void deallocate(pointer p, size_type const n) const
        {
            free(p);
        }

        template<typename U>
        pointer allocate(size_type const n, U const *) const
        {
            allocate(n);
        }
    };
}
#endif // pMR_MISC_ALLOCATOR_H
