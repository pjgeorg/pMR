#include "context.hpp"
#include <stdexcept>
#include "../../misc/string.hpp"

pMR::verbs::Context::Context(Device const &device)
{
    mContext = ibv_open_device(const_cast<ibv_device*>(device.get()));
    if(!mContext)
    {
        throw std::runtime_error(toString("pMR: Could not open device",
                    device.getName(), "."));
    }
}

pMR::verbs::Context::~Context()
{
    ibv_close_device(mContext);
}

ibv_context* pMR::verbs::Context::get()
{
    return mContext;
}

ibv_context const* pMR::verbs::Context::get() const
{
    return mContext;
}
