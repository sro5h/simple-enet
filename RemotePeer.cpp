#include "RemotePeer.hpp"

#include <enet/enet.h>

RemotePeer::RemotePeer()
        : peer(nullptr)
{
}

bool RemotePeer::operator==(const RemotePeer& other) const
{
        return other.id == this->id;
}
