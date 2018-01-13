#ifndef _EVENT_HPP_
#define _EVENT_HPP_

#include "Packet.hpp"

typedef struct _ENetEvent ENetEvent;

struct Event
{
        enum class Type : Uint16
        {
                Connect,
                Disconnect,
                Receive,
        } type;

        Uint16 incomingId;
        Uint16 outgoingId;
        Packet packet;
        std::string address;
        Uint16 port;
};

void toEvent(Event& event, const ENetEvent& enetEvent);

#endif // _EVENT_HPP_
