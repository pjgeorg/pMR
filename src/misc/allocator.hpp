#ifndef pMR_MISC_ALLOCATOR_H
#define pMR_MISC_ALLOCATOR_H

#include <cstdint>
#include <memory>
#include <limits>
#include <stdexcept>

namespace pMR
{
    template<typename T, std::size_t Alignment>
    class AlignedAllocator
    {
        public:
            using value_type = T;
            using reference = T&;
            using const_reference = T const&;
            using pointer = T*;
            using const_pointer = T const*;
            using size_type = std::size_t;
            using difference_type = std::ptrdiff_t;

            template <class U>
            struct rebind
            {
                using other = AlignedAllocator<U, Alignment>;
            };

            AlignedAllocator() = default;
            AlignedAllocator(const AlignedAllocator&) = default;
            AlignedAllocator(AlignedAllocator&&) = default;
            template <typename U>
            AlignedAllocator(AlignedAllocator<U, Alignment> const&) { }
            AlignedAllocator& operator=(const AlignedAllocator&) = delete;
            AlignedAllocator& operator=(AlignedAllocator&&) = delete;
            ~AlignedAllocator() = default;

            pointer address(reference r)
            {
                return &r;
            }
            const_pointer address(const_reference r)
            {
                return &r;
            }
            size_type max_size() const
            {
                return std::numeric_limits<size_type>::max() / sizeof(T);
            }
            
            bool operator==(const AlignedAllocator &rhs)
            {
                return !(*this == rhs);
            }
            bool operator!=(const AlignedAllocator&)
            {
                return true;
            }

            void construct(pointer const p, const_reference r) const
            {
                new (p) value_type(r);
            }
            void destroy(pointer const p) const
            {
                p->~value_type();
            }

            pointer allocate(size_type const n)
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

            void deallocate(pointer p, size_type const n)
            {
                free(p);
            }

            template <typename U>
            pointer allocate(size_type const n, U const*) const
            {
                allocate(n);
            }
    };
}
#endif // pMR_MISC_ALLOCATOR_H
