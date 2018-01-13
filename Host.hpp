#ifndef _HOST_HPP_
#define _HOST_HPP_

#include "Event.hpp"
#include "Packet.hpp"

typedef struct _ENetHost ENetHost;

class Host final
{
public:
        explicit Host();
        ~Host();

        bool create(const std::string& address, Uint16 port, std::size_t maxPeers);
        bool pollEvent(Event& event) const;
        std::size_t getConnectedPeerCount() const;

        void broadcast(const Packet& packet);

private:
        ENetHost* mHost;
};

#endif // _HOST_HPP_
