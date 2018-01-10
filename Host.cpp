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
                switch (enetEvent.type)
                {
                        case ENET_EVENT_TYPE_CONNECT:
                        {
                                onConnect(enetEvent, event);
                        } break;

                        case ENET_EVENT_TYPE_DISCONNECT:
                        {
                                onDisconnect(enetEvent, event);
                        } break;

                        case ENET_EVENT_TYPE_RECEIVE:
                        {
                                onReceive(enetEvent, event);
                        } break;
                }

                return true;
        }

        return false;
}

void Host::broadcast(const sf::Packet& packet)
{
        if (mHost)
        {
                ENetPacket* enetPacket = enet_packet_create(packet.getData(),
                                packet.getDataSize(), 0);

                enet_host_broadcast(mHost, 0, enetPacket);
        }
}

void Host::onConnect(const ENetEvent& enetEvent, Event& event) const
{
        event.type = Event::Type::Connect;
        event.incomingId = enetEvent.peer->incomingPeerID;
        event.outgoingId = enetEvent.peer->outgoingPeerID;
        // TODO: convert ip
        event.port = enetEvent.peer->address.port;
}

void Host::onDisconnect(const ENetEvent& enetEvent, Event& event) const
{
        event.type = Event::Type::Disconnect;
        event.incomingId = enetEvent.peer->incomingPeerID;
        event.outgoingId = enetEvent.peer->outgoingPeerID;
        // TODO: convert ip
        event.port = enetEvent.peer->address.port;
}

void Host::onReceive(const ENetEvent& enetEvent, Event& event) const
{
        event.type = Event::Type::Receive;
        event.incomingId = enetEvent.peer->incomingPeerID;
        event.outgoingId = enetEvent.peer->outgoingPeerID;
        // TODO: convert ip
        event.port = enetEvent.peer->address.port;

        // Copy byte data to Event::packet
        event.packet.append((void*)enetEvent.packet->data, enetEvent.packet->dataLength);

        enet_packet_destroy(enetEvent.packet);
}
