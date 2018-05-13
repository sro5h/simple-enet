#include "Host.hpp"
#include <enet/enet.h>
#include <cassert>

std::string convertAddress(const ENetAddress& address);

Host::Host()
        : mHost(nullptr)
{
}

Host::~Host()
{
        destroy();
}

bool Host::create(const std::string& address, Uint16 port, std::size_t connections)
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

        mHost = enet_host_create(&enetAddress, connections, 2, 0, 0);

        return mHost != nullptr;
}

void Host::destroy()
{
        if (mHost)
        {
                for (std::size_t i = 0; i < mHost->peerCount; ++i)
                {
                        enet_peer_disconnect_now(&mHost->peers[i], 0);
                }

                enet_host_destroy(mHost);
                mHost = nullptr;
        }
}

bool Host::connect(const std::string& address, Uint16 port)
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

        ENetPeer* peer = enet_host_connect(mHost, &enetAddress, 2, 0);

        return peer != nullptr;
}

void Host::disconnect(const Peer& peer)
{
        assert(mHost);

        enet_peer_disconnect(peer.peer, 0);
}

void Host::disconnectAll()
{
        assert(mHost);

        for (std::size_t i = 0; i < mHost->peerCount; ++i)
        {
                enet_peer_disconnect(&mHost->peers[i], 0);
        }
}

bool Host::pollEvent(Event& event)
{
        assert(mHost);

        ENetEvent enetEvent;

        if (enet_host_service(mHost, &enetEvent, 0) > 0)
        {
                convertENetEvent(enetEvent, event);

                return true;
        }

        return false;
}

void Host::flush()
{
        assert(mHost);

        enet_host_flush(mHost);
}

std::size_t Host::getConnectionCount() const
{
        return (mHost) ? mHost->connectedPeers : 0;
}

void Host::broadcast(const Packet& packet)
{
        assert(mHost);

        ENetPacket* enetPacket = toENetPacket(packet);

        enet_host_broadcast(mHost, 0, enetPacket);
}

void Host::broadcastExcept(const Peer& peer, const Packet& packet)
{
        assert(mHost);
        assert(peer.peer);

        ENetPacket* enetPacket = toENetPacket(packet);

        for (std::size_t i = 0; i < mHost->peerCount; ++i)
        {
                if (mHost->peers[i].state != ENET_PEER_STATE_CONNECTED)
                        continue;

                if (&mHost->peers[i] != peer.peer)
                {
                        enet_peer_send(&mHost->peers[i], 0, enetPacket);
                }
        }

        if (enetPacket->referenceCount == 0)
        {
                enet_packet_destroy(enetPacket);
        }
}

bool Host::send(const Peer& peer, const Packet& packet)
{
        assert(mHost);
        assert(peer.peer);

        ENetPacket* enetPacket = toENetPacket(packet);

        int rc = enet_peer_send(peer.peer, 0, enetPacket);

        if (enetPacket->referenceCount == 0)
        {
                enet_packet_destroy(enetPacket);
        }

        return rc == 0;
}

Host::operator bool() const
{
        return mHost != nullptr;
}

void Host::convertENetEvent(const ENetEvent& enetEvent, Event& event)
{
        assert(enetEvent.type != ENET_EVENT_TYPE_NONE);

        event = Event();
        ENetPeer* enetPeer = enetEvent.peer;
        convertENetPeer(*enetPeer, event.peer);

        if (enetEvent.type == ENET_EVENT_TYPE_CONNECT)
        {
                event.type = Event::Type::Connect;

                assert(mPeerData.find(enetPeer) == mPeerData.end());
                mPeerData[enetPeer].outgoingId = event.peer.outgoingId;
                mPeerData[enetPeer].connectId = event.peer.connectId;
        }
        else if (enetEvent.type == ENET_EVENT_TYPE_DISCONNECT)
        {
                event.type = Event::Type::Disconnect;

                assert(mPeerData.find(enetPeer) != mPeerData.end());
                event.peer.outgoingId = mPeerData[enetPeer].outgoingId;
                event.peer.connectId = mPeerData[enetPeer].connectId;
                mPeerData.erase(enetPeer);
        }
        else if (enetEvent.type == ENET_EVENT_TYPE_RECEIVE)
        {
                event.type = Event::Type::Receive;
                event.packet.append((void*)enetEvent.packet->data,
                                    enetEvent.packet->dataLength);

                assert(mPeerData.find(enetPeer) != mPeerData.end());
                enet_packet_destroy(enetEvent.packet);
        }
}

void Host::convertENetPeer(ENetPeer& enetPeer, Peer& peer)
{
        peer.peer = &enetPeer;
        peer.incomingId = enetPeer.incomingPeerID;
        peer.outgoingId = enetPeer.outgoingPeerID;
        peer.connectId = enetPeer.connectID;
        peer.address = convertAddress(enetPeer.address);
        peer.port = enetPeer.address.port;
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
