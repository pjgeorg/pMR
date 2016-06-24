#ifndef pMR_CLUSTERS_IDATACOOL_CONNECTION_H
#define pMR_CLUSTERS_IDATACOOL_CONNECTION_H

#include <memory>
#include <cstdint>
#include "provider.hpp"

namespace pMR
{
    class SendMemoryWindow;
    class RecvMemoryWindow;
    class Target;

    class Connection
    {
        friend SendMemoryWindow;
        friend RecvMemoryWindow;
        public:
            Connection(Target const &target);
            Connection(const Connection&) = delete;
            Connection(Connection&&) = default;
            Connection& operator=(const Connection&) = delete;
            Connection& operator=(Connection&&) = default;
            ~Connection() = default;
        private:
            Provider mProvider = Provider::null;
            std::shared_ptr<null::Connection> mNull;
            std::shared_ptr<self::Connection> mSelf;
            std::shared_ptr<loop::Connection> mLoop;
            std::shared_ptr<cma::Connection> mCMA;
            std::shared_ptr<verbs::Connection> mVerbs;
            void setProvider(Target const &target);
            Provider detectProvider(Target const &target) const;
            void connect(Target const &target);
            void connectNull(Target const &target);
            void connectSelf(Target const &target);
            void connectLoop(Target const &target);
            void connectCMA(Target const &target);
            void connectVerbs(Target const &target);
    };
}
#endif // pMR_CLUSTERS_IDATACOOL_CONNECTION_H
