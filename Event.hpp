#ifndef _EVENT_HPP_
#define _EVENT_HPP_

#include <SFML/Network/Packet.hpp>

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
        std::string ip;
        sf::Uint16 port;
};

#endif // _EVENT_HPP_
