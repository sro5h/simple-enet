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

void convertConnect(const ENetEvent& enetEvent, Event& event);
void convertDisconnect(const ENetEvent& enetEvent, Event& event);
void convertReceive(const ENetEvent& enetEvent, Event& event);

#endif // _EVENT_HPP_
