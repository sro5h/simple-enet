#include "Packet.hpp"

#include <enet/enet.h>

ENetPacket* toENetPacket(const sf::Packet& packet, Packet::Flag flag)
{
        sf::Uint32 enetFlags = 0;
        if (flag == Packet::Flag::Reliable)
        {
                enetFlags |= ENET_PACKET_FLAG_RELIABLE;
        }
        else if (flag == Packet::Flag::Unreliable)
        {
                enetFlags |= ENET_PACKET_FLAG_UNRELIABLE_FRAGMENT;
        }
        else if (flag == Packet::Flag::Unsequenced)
        {
                enetFlags |= ENET_PACKET_FLAG_UNRELIABLE_FRAGMENT
                        | ENET_PACKET_FLAG_UNSEQUENCED;
        }

        return enet_packet_create(packet.getData(), packet.getDataSize(),
                        enetFlags);
}
