#ifndef EVENT_HPP_INCLUDED
#define EVENT_HPP_INCLUDED

#include "Packet.hpp"
#include "Peer.hpp"

/**
 * Holds all relevant data of the according ENetEvent.
 */
struct Event final
{
        /**
         * The type of the Event, maps to an ENetEventType.
         */
        enum class Type : Uint16
        {
                Connect,
                Disconnect,
                Receive,
        } type;

        Peer peer;
        Packet packet;
};

#endif // EVENT_HPP_INCLUDED
