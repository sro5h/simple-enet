#ifndef PEER_HPP_INCLUDED
#define PEER_HPP_INCLUDED

#include "Types.hpp"
#include <string>

enum class State
{
        Zombie,
        Disconnected,
        Disconnecting,
        AcknowlegdingDisconnect,
        DisconnectLater,
        Connected,
        Connecting,
        AcknowledgingConnect,
        PendingConnect,
        Succeeded,
};

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

        State getState();

public:
        /**
         * Compares the internal ENetPeer pointer to test whether both Peers
         * are the same.
         * @param other The other Peer to compare this to
         * @return True if both are the same
         */
        bool operator==(const Peer& other) const;

        /**
         * Compares the internal ENetPeer pointer to test whether this peer is
         * less than the other.
         * @param other The other Peer to compare this to
         * @return True if this peer is less than the other
         */
        bool operator<(const Peer& other) const;

        /**
         * Returns whether the Peer is in a valid state.
         * This means the associated ENetPeer exists.
         * @return True if the Peer is valid
         */
        explicit operator bool() const;

public:
        std::string address;
        Uint16 port;
        Uint16 incomingId;
        Uint16 outgoingId;
        Uint32 connectId;

private:
        ENetPeer* peer;

        friend class Host;
        friend void convertPeer(Peer&, ENetPeer&);
};

#endif // PEER_HPP_INCLUDED
