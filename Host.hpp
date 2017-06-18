#ifndef HOST_HPP
#define HOST_HPP

#include <enet/enet.h>
#include <sstream>

#define PORT_ANY -1

std::string convertAddress(const ENetAddress& address);

/**
 * Holds all the relevant ENetEvent data.
 */
struct Event {
        enum EventType
        {
                CONNECTED,
                RECEIVED,
                DISCONNECTED
        };

        EventType type;
        unsigned int connectId;
        std::string data;
        std::string ip;
        unsigned int port;
};

/**
 * Base class for both clients and servers.
 */
class Host {
protected:
        ENetHost* host;

        /**
         * Gets called after an ENetPeer connects or a connection attempt was
         * successful.
         */
        void onConnect(const ENetEvent& enetEvent, Event& event)
        {
                event.type = Event::CONNECTED;

                /* Store the connectID in ENetPeer::data */
                int* id = new int(enetEvent.peer->connectID);
                enetEvent.peer->data = id;
                event.connectId = *id;

                event.ip = convertAddress(enetEvent.peer->address);
                event.port = enetEvent.peer->address.port;
        }

        /**
         * Gets called if an ENetPacket is received and converts the data to a
         * std::string.
         */
        void onReceive(const ENetEvent& enetEvent, Event& event)
        {
                event.type = Event::RECEIVED;
                event.connectId = enetEvent.peer->connectID;

                /* Convert the sent data to a std::string */
                std::stringstream tmp;
                tmp << enetEvent.packet->data;
                event.data = tmp.str();

                enet_packet_destroy(enetEvent.packet);
        }

        /**
         * Gets called after an ENetPeer disconnects or a disconnection attempt
         * was successful.
         */
        void onDisconnect(const ENetEvent& enetEvent, Event& event)
        {
                event.type = Event::DISCONNECTED;
                event.connectId = *(int*) enetEvent.peer->data;

                /* TODO: Free data if the peer disconnects */
        }

public:
        /**
         * Destroys the ENetHost.
         */
        ~Host()
        {
                enet_host_destroy(host);
        }

        /**
         * Creates an ENetHost on the specified port or if port equals PORT_ANY
         * on a availlable port.
         *
         * @return true if successful, false otherwise
         */
        bool create(int port)
        {
                if (port == PORT_ANY) {
                        host = enet_host_create(NULL, 1, 2, 0, 0);

                } else {
                        ENetAddress address;

                        address.host = ENET_HOST_ANY;
                        address.port = port;
                        host = enet_host_create(&address, 32, 2, 0, 0);
                }

                return host != NULL;
        }

        /**
         * Update the ENetHost and convert pending ENetEvents to the Event
         * struct.
         *
         * @return false if no ENetEvents was availlable, true otherwise
         */
        bool pollEvent(Event& event)
        {
                ENetEvent enetEvent;

                if (enet_host_service(host, &enetEvent, 0) > 0) {
                        if (enetEvent.type == ENET_EVENT_TYPE_CONNECT) {
                                onConnect(enetEvent, event);

                        } else if (enetEvent.type == ENET_EVENT_TYPE_RECEIVE) {
                                onReceive(enetEvent, event);

                        } else if (enetEvent.type == ENET_EVENT_TYPE_DISCONNECT) {
                                onDisconnect(enetEvent, event);
                        }

                        return true;
                }

                return false;
        }
};

/**
 * Convert ENetAddress::host to a std::string.
 */
std::string convertAddress(const ENetAddress& address)
{
        char buffer[32];
        std::string str = "";

        if (enet_address_get_host_ip(&address, buffer, 32) == 0) {
                str = buffer;
        }

        return str;
}

#endif /* Header guard */
