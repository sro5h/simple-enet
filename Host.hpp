#include <enet/enet.h>
#include <sstream>

#define PORT_ANY -1

std::string convertAddress(const ENetAddress& address);

struct Event
{
        enum EventType
        {
                Connected,
                Received,
                Disconnected
        };

        EventType type;
        unsigned int peerId;
        std::string data;
        std::string ip;
        unsigned int port;
};

class Host
{
protected:
        ENetHost* host;

        void onConnect(const ENetEvent& enetEvent, Event& event)
        {
                event.type = Event::Connected;

                /* Store the connectID in ENetPeer::data */
                int* id = new int(enetEvent.peer->connectID);
                enetEvent.peer->data = id;
                event.peerId = *id;

                event.ip = convertAddress(enetEvent.peer->address);
                event.port = enetEvent.peer->address.port;
        }

        void onReceive(const ENetEvent& enetEvent, Event& event)
        {
                event.type = Event::Received;
                event.peerId = enetEvent.peer->connectID;

                /* Convert the sent data to a std::string */
                std::stringstream tmp;
                tmp << enetEvent.packet->data;
                event.data = tmp.str();

                enet_packet_destroy(enetEvent.packet);
        }

        void onDisconnect(const ENetEvent& enetEvent, Event& event)
        {
                event.type = Event::Disconnected;
                event.peerId = *(int*) enetEvent.peer->data;

                /* TODO: Free data if the peer disconnects */
        }

public:
        Host(int port)
        {
                if(port == PORT_ANY) {
                        host = enet_host_create(NULL, 1, 2, 0, 0);

                } else {
                        ENetAddress address;

                        address.host = ENET_HOST_ANY;
                        address.port = port;
                        host = enet_host_create(&address, 32, 2, 0, 0);
                }

                if(host == NULL) {
                        /* TODO: Handle error */
                }
        }

        ~Host()
        {
                enet_host_destroy(host);
        }

        bool pollEvent(Event& event)
        {
                ENetEvent enetEvent;

                if(enet_host_service(host, &enetEvent, 0) > 0) {
                        if(enetEvent.type == ENET_EVENT_TYPE_CONNECT) {
                                onConnect(enetEvent, event);

                        } else if(enetEvent.type == ENET_EVENT_TYPE_RECEIVE) {
                                onReceive(enetEvent, event);

                        } else if(enetEvent.type == ENET_EVENT_TYPE_DISCONNECT) {
                                onDisconnect(enetEvent, event);
                        }

                        return true;
                }

                return false;
        }
};

std::string convertAddress(const ENetAddress& address)
{
        char buffer[32];
        std::string str = "";

        if(enet_address_get_host_ip(&address, buffer, 32) == 0) {
                str = buffer;
        }

        return str;
}
