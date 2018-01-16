#ifndef _REMOTE_PEER_HPP_
#define _REMOTE_PEER_HPP_

#include "Types.hpp"

#include <string>

struct Peer final
{
public:
        explicit Peer();

public:
        bool operator==(const Peer& other) const;

public:
        std::string address;
        Uint16 port;
        Uint16 id;

private:
        ENetPeer* peer;

        friend class Host;
        friend void convertPeer(Peer&, const ENetPeer&);
};

#endif // _REMOTE_PEER_HPP_
