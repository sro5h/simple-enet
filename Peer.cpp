#include "Peer.hpp"

#include <enet/enet.h>

#include <cassert>

Peer::Peer()
        : mHost(nullptr)
        , mRemoteHost(nullptr)
{
}

Peer::~Peer()
{
        if (mHost)
        {
                if (mRemoteHost)
                {
                        enet_peer_disconnect(mRemoteHost, 0);
                        enet_host_flush(mHost);
                }

                enet_host_destroy(mHost);
        }
}

bool Peer::create()
{
        assert(mHost == nullptr);

        mHost = enet_host_create(nullptr, 1, 2, 0, 0);

        return mHost != nullptr;
}

bool Peer::connect(const std::string& address, Uint16 port)
{
        assert(mHost);

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

        mRemoteHost = enet_host_connect(mHost, &enetAddress, 2, 0);

        return mRemoteHost != nullptr;
}

bool Peer::pollEvent(Event& event) const
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

void Peer::send(const Packet& packet)
{
        assert(mHost);

        ENetPacket* enetPacket = toENetPacket(packet);

        enet_peer_send(mRemoteHost, 0, enetPacket);

        if (enetPacket->referenceCount == 0)
        {
                enet_packet_destroy(enetPacket);
        }
}
