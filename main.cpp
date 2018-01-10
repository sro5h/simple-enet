#include "Host.hpp"
#include "Peer.hpp"

#include <enet/enet.h>

#include <iostream>

#define PORT 42323

int main(int argc, char** argv)
{
        if (enet_initialize() != 0)
        {
                std::cout << "Could not initialize enet." << std::endl;
                return EXIT_FAILURE;
        }

        if (argc != 2) {
                std::cout << "Usage: " << argv[0] << " [c,s]" << std::endl;
                return EXIT_FAILURE;
        }

        std::string mode(argv[1]);

        if (mode == "s")
        {
                Host host;

                if (!host.create("localhost", PORT, 5))
                {
                        std::cout << "Could not create host." << std::endl;
                        return EXIT_FAILURE;
                }

                std::cout << "Host created." << std::endl;

                while (true)
                {
                        Event event;
                        while (host.pollEvent(event))
                        {
                                if (event.type == Event::Type::Connect)
                                {
                                        std::cout << "New peer[id="
                                                << event.incomingId << "]"
                                                << std::endl;
                                }
                                else if (event.type == Event::Type::Disconnect)
                                {
                                        std::cout << "Peer[id="
                                                << event.incomingId
                                                << "] disconnected"
                                                << std::endl;
                                }
                                else if (event.type == Event::Type::Receive)
                                {
                                        std::string msg;
                                        event.packet >> msg;

                                        std::cout << "Received[id="
                                                << event.incomingId << "]: "
                                                << msg << std::endl;
                                }
                        }
                }
        }
        else
        {
                Peer peer;

                if (!peer.create())
                {
                        std::cout << "Could not create peer." << std::endl;
                        return EXIT_FAILURE;
                }

                std::cout << "Peer created." << std::endl;

                // Try to connect to the host
                if (!peer.connect("localhost", PORT))
                {
                        std::cout << "Could not connect to the server." << std::endl;
                        return EXIT_FAILURE;
                }

                while (true)
                {
                        Event event;
                        while (peer.pollEvent(event))
                        {
                                if (event.type == Event::Type::Connect)
                                {
                                        std::cout << "Connected[id="
                                                << event.incomingId << "]"
                                                << std::endl;

                                        sf::Packet packet;
                                        packet << "ping";
                                        peer.send(packet);
                                }
                                else if (event.type == Event::Type::Disconnect)
                                {
                                        std::cout << "Disconnected[id="
                                                << event.incomingId << "]"
                                                << std::endl;
                                }
                                else if (event.type == Event::Type::Receive)
                                {
                                        std::cout << "Received[id="
                                                << event.incomingId << "]"
                                                << std::endl;
                                }
                        }
                }
        }

        enet_deinitialize();
        return EXIT_SUCCESS;
}
