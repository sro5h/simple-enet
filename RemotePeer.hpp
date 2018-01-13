#ifndef _REMOTE_PEER_HPP_
#define _REMOTE_PEER_HPP_

#include "Packet.hpp"

typedef struct _ENetPeer ENetPeer;

struct RemotePeer final
{
public:
        bool operator==(const RemotePeer& other) const;

public:
        std::string address;
        Uint16 port;
        Uint16 id;
};

#endif // _REMOTE_PEER_HPP_
