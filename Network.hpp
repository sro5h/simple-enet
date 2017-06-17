#include <enet/enet.h>
#include <sstream>
#include <iostream>

#define EVENT_TYPE_NONE -1
#define EVENT_TYPE_CONNECT 0
#define EVENT_TYPE_RECEIVE 1
#define EVENT_TYPE_DISCONNECT 2

struct Event
{
        int type;
        unsigned int peerId;
        std::string data;
};

class Server
{
private:
        ENetHost* host;

        void handleEvent(const ENetEvent& enetEvent, Event& event)
        {
                if(enetEvent.type == ENET_EVENT_TYPE_CONNECT) {
                        event.type = EVENT_TYPE_CONNECT;
                        event.peerId = enetEvent.peer->connectID;

                } else if(enetEvent.type == ENET_EVENT_TYPE_RECEIVE) {
                        event.type = EVENT_TYPE_RECEIVE;
                        event.peerId = enetEvent.peer->connectID;

                        std::stringstream in;
                        in << enetEvent.packet->data;
                        event.data = in.str();

                        enet_packet_destroy(enetEvent.packet);

                } else if(enetEvent.type == ENET_EVENT_TYPE_DISCONNECT) {
                        event.type = EVENT_TYPE_DISCONNECT;
                        event.peerId = enetEvent.data;
                }
        }

public:
        Server(int port)
        {
                ENetAddress address;

                address.host = ENET_HOST_ANY;
                address.port = port;
                host = enet_host_create(&address, 32, 2, 0, 0);

                if(host == NULL) {
                        /* TODO: Handle error */
                }
        }

        ~Server()
        {
                enet_host_destroy(host);
        }

        bool pollEvent(Event& event)
        {
                ENetEvent enetEvent;

                if(enet_host_service(host, &enetEvent, 0) > 0) {
                        handleEvent(enetEvent, event);

                        return true;
                }

                return false;
        }
};

class Client
{
private:
        ENetHost* host;
        ENetPeer* server;

        void handleEvent(const ENetEvent& enetEvent, Event& event)
        {
                if(enetEvent.type == ENET_EVENT_TYPE_CONNECT) {
                        event.type = EVENT_TYPE_CONNECT;
                        event.peerId = enetEvent.peer->connectID;

                } else if(enetEvent.type == ENET_EVENT_TYPE_RECEIVE) {
                        event.type = EVENT_TYPE_RECEIVE;
                        event.peerId = enetEvent.peer->connectID;

                        std::stringstream in;
                        in << enetEvent.packet->data;
                        event.data = in.str();

                        enet_packet_destroy(enetEvent.packet);

                } else if(enetEvent.type == ENET_EVENT_TYPE_DISCONNECT) {
                        event.type = EVENT_TYPE_DISCONNECT;
                        event.peerId = enetEvent.peer->connectID;
                }
        }

public:
        Client()
        {
                host = enet_host_create(NULL, 1, 2, 0, 0);

                if(host == NULL) {
                        /* TODO: Handle error */
                }
        }

        ~Client()
        {
                enet_host_destroy(host);
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

        bool pollEvent(Event& event)
        {
                ENetEvent enetEvent;

                if(enet_host_service(host, &enetEvent, 0) > 0) {
                        handleEvent(enetEvent, event);

                        return true;
                }

                return false;
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
