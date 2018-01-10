#ifndef _PEER_HPP_
#define _PEER_HPP_

#include "Event.hpp"

typedef struct _ENetHost ENetHost;
typedef struct _ENetPeer ENetPeer;

class Peer final
{
public:
        explicit Peer();
        ~Peer();

        bool create();
        bool connect(const std::string& ip, sf::Uint16 port);
        bool pollEvent(Event& event) const;

        void send(const sf::Packet& packet);

private:
        void onConnect(const ENetEvent& enetEvent, Event& event) const;
        void onDisconnect(const ENetEvent& enetEvent, Event& event) const;
        void onReceive(const ENetEvent& ENetEvent, Event& event) const;

private:
        ENetHost* mHost;
        ENetPeer* mRemoteHost;
};

#endif // _PEER_HPP_
