#include "Peer.hpp"

#include <enet/enet.h>

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
        if (mHost)
        {
                // The host already exists
                return false;
        }

        mHost = enet_host_create(nullptr, 1, 2, 0, 0);

        return mHost != nullptr;
}

bool Peer::connect(const std::string& address, sf::Uint16 port)
{
        if (!mHost) return false;

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
        if (!mHost) return false;

        ENetEvent enetEvent;

        if (enet_host_service(mHost, &enetEvent, 0) > 0)
        {
                switch (enetEvent.type)
                {
                        case ENET_EVENT_TYPE_CONNECT:
                        {
                                convertConnect(enetEvent, event);
                        } break;

                        case ENET_EVENT_TYPE_DISCONNECT:
                        {
                                convertDisconnect(enetEvent, event);
                        } break;

                        case ENET_EVENT_TYPE_RECEIVE:
                        {
                                convertReceive(enetEvent, event);
                        } break;
                }

                return true;
        }

        return false;
}

void Peer::send(const sf::Packet& packet, Packet::Flag flag)
{
        if (mHost)
        {
                ENetPacket* enetPacket = toENetPacket(packet, flag);

                enet_peer_send(mRemoteHost, 0, enetPacket);
        }
}
