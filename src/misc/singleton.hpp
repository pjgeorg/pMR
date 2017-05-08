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

#ifndef pMR_MISC_SINGLETON_H
#define pMR_MISC_SINGLETON_H

namespace pMR
{
    template<class T>
    class Singleton
    {
    public:
        static T &Instance()
        {
            static T theInstance;
            return theInstance;
        }

        template<typename... Types>
        static T &Instance(Types &... args)
        {
            static T theInstance(args...);
            return theInstance;
        }

    private:
        Singleton() = default;
        Singleton(Singleton const &) = delete;
        Singleton(Singleton &&) = delete;
        Singleton &operator=(Singleton const &) = delete;
        Singleton &operator=(Singleton &&) = delete;
        ~Singleton() = default;
    };
}
#endif // pMR_MISC_SINGLETON_H
