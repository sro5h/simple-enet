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
        mHost = enet_host_create(nullptr, 1, 2, 0, 0);

        return mHost != nullptr;
}

bool Peer::connect(const std::string& ip, sf::Uint16 port)
{
        ENetAddress address;
        enet_address_set_host(&address, ip.c_str());
        address.port = port;

        mRemoteHost = enet_host_connect(mHost, &address, 2, 0);

        return mRemoteHost != nullptr;
}

bool Peer::pollEvent(Event& event) const
{
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

void Peer::send(const sf::Packet& packet)
{
        ENetPacket* enetPacket = enet_packet_create(packet.getData(),
                        packet.getDataSize(), 0);

        enet_peer_send(mRemoteHost, 0, enetPacket);
}

void Peer::onConnect(const ENetEvent& enetEvent, Event& event) const
{
        event.type = Event::Type::Connect;
        event.incomingId = enetEvent.peer->incomingPeerID;
        event.outgoingId = enetEvent.peer->outgoingPeerID;
        // TODO: convert ip
        event.port = enetEvent.peer->address.port;
}

void Peer::onDisconnect(const ENetEvent& enetEvent, Event& event) const
{
        event.type = Event::Type::Disconnect;
        event.incomingId = enetEvent.peer->incomingPeerID;
        event.outgoingId = enetEvent.peer->outgoingPeerID;
        // TODO: convert ip
        event.port = enetEvent.peer->address.port;
}

void Peer::onReceive(const ENetEvent& enetEvent, Event& event) const
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
