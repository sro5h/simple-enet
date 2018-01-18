#ifndef PEER_HPP
#define PEER_HPP

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
        friend void toEvent(Event&, const ENetEvent&);
};

#endif // PEER_HPP
