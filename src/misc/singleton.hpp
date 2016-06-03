#ifndef pMR_MISC_SINGLETON_H
#define pMR_MISC_SINGLETON_H

namespace pMR
{
    template<class T>
    class Singleton
    {
        public:
            static T& Instance()
            {
                static T theInstance;
                return theInstance;
            }
            template<typename ...Types>
            static T& Instance(Types &...args)
            {
                static T theInstance(args...);
                return theInstance;
            }
        private:
            Singleton() = default;
            Singleton(const Singleton&) = delete;
            Singleton(Singleton&&) = delete;
            Singleton& operator=(const Singleton&) = delete;
            Singleton& operator=(Singleton&&) = delete;
            ~Singleton() = default;
    };
}
#endif // pMR_MISC_SINGLETON_H
