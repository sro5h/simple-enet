#include "Event.hpp"

#include <enet/enet.h>

void convertConnect(Event& event, const ENetEvent& enetEvent);
void convertDisconnect(Event& event, const ENetEvent& enetEvent);
void convertReceive(Event& event, const ENetEvent& enetEvent);
std::string convertAddress(const ENetAddress& address);

void toEvent(Event& event, const ENetEvent& enetEvent)
{
        switch (enetEvent.type)
        {
                case ENET_EVENT_TYPE_CONNECT:
                {
                        convertConnect(event, enetEvent);
                } break;

                case ENET_EVENT_TYPE_DISCONNECT:
                {
                        convertDisconnect(event, enetEvent);
                } break;

                case ENET_EVENT_TYPE_RECEIVE:
                {
                        convertReceive(event, enetEvent);
                } break;
        }
}

void convertConnect(Event& event, const ENetEvent& enetEvent)
{
        event.type = Event::Type::Connect;
        event.incomingId = enetEvent.peer->incomingPeerID;
        event.outgoingId = enetEvent.peer->outgoingPeerID;
        event.address = convertAddress(enetEvent.peer->address);
        event.port = enetEvent.peer->address.port;
}

void convertDisconnect(Event& event, const ENetEvent& enetEvent)
{
        event.type = Event::Type::Disconnect;
        event.incomingId = enetEvent.peer->incomingPeerID;
        event.outgoingId = enetEvent.peer->outgoingPeerID;
        event.address = convertAddress(enetEvent.peer->address);
        event.port = enetEvent.peer->address.port;
}

void convertReceive(Event& event, const ENetEvent& enetEvent)
{
        event.type = Event::Type::Receive;
        event.incomingId = enetEvent.peer->incomingPeerID;
        event.outgoingId = enetEvent.peer->outgoingPeerID;
        event.address = convertAddress(enetEvent.peer->address);
        event.port = enetEvent.peer->address.port;

        // Copy byte data to Event::packet
        event.packet.append((void*)enetEvent.packet->data, enetEvent.packet->dataLength);

        enet_packet_destroy(enetEvent.packet);
}

std::string convertAddress(const ENetAddress& address)
{
        char buffer[64];
        std::string str;

        if (enet_address_get_host_ip(&address, buffer, sizeof(buffer)) == 0)
        {
                str = buffer;
        }

        return str;
}
