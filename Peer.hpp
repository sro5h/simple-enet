#ifndef PEER_HPP
#define PEER_HPP

#include "Types.hpp"

#include <string>

/**
 * Represents a connection with a Host.
 */
struct Peer final
{
public:
        /**
         * Initializes the Peer to an invalid state.
         */
        explicit Peer();

public:
        /**
         * Compares the Peer::id to test whether both Peers are the same.
         * @param other The other Peer to compare this to
         * @return True if both are the same
         */
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
