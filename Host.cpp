#include "Host.hpp"

#include <enet/enet.h>

#include <cassert>

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

bool Host::create(const std::string& address, Uint16 port, std::size_t maxPeers)
{
        assert(mHost == nullptr);

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
        assert(mHost);

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

void Host::broadcast(const Packet& packet)
{
        assert(mHost);

        ENetPacket* enetPacket = toENetPacket(packet);

        enet_host_broadcast(mHost, 0, enetPacket);
}

void Host::broadcastExcept(const RemotePeer& peer, const Packet& packet)
{
        assert(mHost);

        ENetPacket* enetPacket = toENetPacket(packet);

        for (std::size_t i = 0; i < mHost->peerCount; ++i)
        {
                if (mHost->peers[i].state != ENET_PEER_STATE_CONNECTED)
                        continue;

                if (mHost->peers[i].incomingPeerID != peer.id)
                {
                        enet_peer_send(&mHost->peers[i], 0, enetPacket);
                }
        }

        if (enetPacket->referenceCount == 0)
        {
                enet_packet_destroy(enetPacket);
        }
}

void Host::send(const RemotePeer& peer, const Packet& packet)
{
        assert(mHost);

        ENetPacket* enetPacket = toENetPacket(packet);

        enet_peer_send(peer.peer, 0, enetPacket);

        if (enetPacket->referenceCount == 0)
        {
                enet_packet_destroy(enetPacket);
        }
}
