#include "Host.hpp"
#include <iostream>


class Server : public Host
{
public:
        Server(int port)
                : Host(port)
        {
        }
};

class Client : public Host
{
private:
        ENetPeer* server;

public:
        Client()
                : Host(PORT_ANY)
        {
        }

        bool connect(const std::string& ip, int port)
        {
                ENetAddress address;

                enet_address_set_host(&address, ip.c_str());
                address.port = port;
                server = enet_host_connect(host, &address, 2, 0);

                if(server == NULL) {
                        /* Couldn't establish connection */
                        return false;
                }

                return true;
        }

        bool send(const std::string& data)
        {
                ENetPacket* packet = enet_packet_create(data.c_str(),
                                data.length() + 1, ENET_PACKET_FLAG_RELIABLE);

                if(enet_peer_send(server, 0, packet) != 0) {
                        /* Packet could not be sent */
                        return false;
                }

                return true;
        }

        bool disconnect()
        {
                std::cout << "Disconnecting from " << server->connectID << std::endl;
                enet_peer_disconnect(server, server->connectID);
                enet_host_flush(host);
        }
};
