#ifndef _HOST_HPP_
#define _HOST_HPP_

#include "Event.hpp"

typedef struct _ENetHost ENetHost;
typedef struct _ENetEvent ENetEvent;

class Host final
{
public:
        explicit Host();
        ~Host();

        bool create(const std::string& address, sf::Uint16 port, std::size_t maxPeers);
        bool pollEvent(Event& event) const;
        std::size_t getConnectedPeerCount() const;

        void broadcast(const sf::Packet& packet);

private:
        void onConnect(const ENetEvent& enetEvent, Event& event) const;
        void onDisconnect(const ENetEvent& enetEvent, Event& event) const;
        void onReceive(const ENetEvent& ENetEvent, Event& event) const;

private:
        ENetHost* mHost;
};

#endif // _HOST_HPP_
