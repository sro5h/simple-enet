#include "Host.hpp"

/**
 * The server implementation.
 */
class Server : public Host {
public:
        /**
         * Send string data to all connected ENetPeers.
         */
        void sendToAll(const std::string& data)
        {
                ENetPacket* packet = enet_packet_create(data.c_str(),
                                data.length() + 1, ENET_PACKET_FLAG_RELIABLE);

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
        bool send(const std::string& data)
        {
                ENetPacket* packet = enet_packet_create(data.c_str(),
                                data.length() + 1, ENET_PACKET_FLAG_RELIABLE);

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
        bool disconnect()
        {
                enet_peer_disconnect(server, server->connectID);
                enet_host_flush(host);
        }
};
