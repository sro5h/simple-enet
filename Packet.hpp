#ifndef _PACKET_HPP_
#define _PACKET_HPP_

#include <SFML/Network/Packet.hpp>

typedef struct _ENetPacket ENetPacket;

namespace Packet
{
enum class Flag : sf::Uint32
{
        Reliable,
        Unreliable,
        Unsequenced,
};
}

ENetPacket* toENetPacket(const sf::Packet& packet, Packet::Flag flag);

#endif // _PACKET_HPP_
