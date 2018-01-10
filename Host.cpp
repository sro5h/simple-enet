#include "Host.hpp"

#include <enet/enet.h>

Host::Host()
        : mHost(nullptr)
{
}

Host::~Host()
{
        if (mHost)
        {
                enet_host_destroy(mHost);
        }
}

bool Host::create(const std::string& address, sf::Uint16 port, std::size_t maxPeers)
{
        if (mHost)
        {
                // The host already exists
                return false;
        }

        ENetAddress enetAddress;
        if (address.empty())
        {
                enetAddress.host = ENET_HOST_ANY;
        }
        else
        {
                if (enet_address_set_host(&enetAddress, address.c_str()) != 0)
                {
                        // Address could not be set
                        return false;
                }
        }
        enetAddress.port = port;

        mHost = enet_host_create(&enetAddress, maxPeers, 2, 0, 0);

        return mHost != nullptr;
}

bool Host::pollEvent(Event& event) const
{
        if (!mHost) return false;

        ENetEvent enetEvent;

        if (enet_host_service(mHost, &enetEvent, 0) > 0)
        {
                toEvent(event, enetEvent);

                return true;
        }

        return false;
}

std::size_t Host::getConnectedPeerCount() const
{
        return (mHost) ? mHost->connectedPeers : 0;
}

void Host::broadcast(const sf::Packet& packet, Packet::Flag flag)
{
        if (mHost)
        {
                ENetPacket* enetPacket = toENetPacket(packet, flag);

                enet_host_broadcast(mHost, 0, enetPacket);
        }
}
