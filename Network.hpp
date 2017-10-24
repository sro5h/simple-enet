#ifndef _NETWORK_HPP
#define _NETWORK_HPP

#include "Host.hpp"

enet_uint32 getPacketFlags(const PacketType& type);

/**
 * The server implementation.
 */
class Server : public Host {
public:
        /**
         * Send string data to all connected ENetPeers.
         */
        void sendToAll(const std::string& data, const PacketType& type) const
        {
                enet_uint32 flags = getPacketFlags(type);

                ENetPacket* packet = enet_packet_create(data.c_str(),
                                data.length() + 1, flags);

                enet_host_broadcast(host, 0, packet);
        }
};

/**
 * The client implementation
 */
class Client : public Host {
private:
        ENetPeer* server;

public:
        /**
         * Connect to a remote ENetHost on ip:port.
         *
         * @return true if successful, false otherwise
         */
        bool connect(const std::string& ip, int port)
        {
                ENetAddress address;

                enet_address_set_host(&address, ip.c_str());
                address.port = port;
                server = enet_host_connect(host, &address, 2, 0);

                if (server == NULL) {
                        /* Couldn't establish connection */
                        return false;
                }

                return true;
        }

        /**
         * Send string data to a connected ENetPeer.
         *
         * @return true if successful, false otherwise
         */
        bool send(const std::string& data, const PacketType& type) const
        {
                enet_uint32 flags = getPacketFlags(type);

                ENetPacket* packet = enet_packet_create(data.c_str(),
                                data.length() + 1, flags);

                if (enet_peer_send(server, 0, packet) != 0) {
                        /* Packet could not be sent */
                        return false;
                }

                return true;
        }

        /**
         * Disconnect from a connected ENetPeer.
         *
         * @return true if successful, false otherwise
         */
        bool disconnect() const
        {
                /* TODO: Indicate that this isn't a timeout */
                enet_peer_disconnect(server, 0);
                enet_host_flush(host);
        }
};

enet_uint32 getPacketFlags(const PacketType& type)
{
        /* Default to unreliable */
        enet_uint32 flags = 0;

        switch (type) {
                case PacketType::Unreliable:
                        flags = 0;
                        break;

                case PacketType::Reliable:
                        flags = ENET_PACKET_FLAG_RELIABLE;
                        break;
        }

        return flags;
}

#endif /* _NETWORK_HPP */
