#ifndef _EVENT_HPP_
#define _EVENT_HPP_

#include <SFML/Network/Packet.hpp>

typedef struct _ENetEvent ENetEvent;

struct Event
{
        enum class Type : sf::Uint32
        {
                Connect,
                Disconnect,
                Receive,
        } type;

        sf::Uint32 incomingId;
        sf::Uint32 outgoingId;
        sf::Packet packet;
        std::string address;
        sf::Uint16 port;
};

void toEvent(Event& event, const ENetEvent& enetEvent);

#endif // _EVENT_HPP_
