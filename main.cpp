#include "Host.hpp"

#include <enet/enet.h>
#include <iostream>

#define SERVER_PORT 42323
#define CLIENT_PORT 53434

int main(int argc, char** argv)
{
        Uint16 port = CLIENT_PORT;

        if (enet_initialize() != 0)
        {
                std::cout << "Could not initialize enet." << std::endl;
                return EXIT_FAILURE;
        }

        if (argc < 2) {
                std::cout << "Usage: " << argv[0] << " [c,s]" << std::endl;
                return EXIT_FAILURE;
        }

        std::string mode(argv[1]);

        if (argc == 3)
        {
                port = std::atoi(argv[2]);
        }

        if (mode == "s")
        {
                Host host;

                if (!host.create("localhost", SERVER_PORT, 5))
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
                                                << event.peer.id << "]"
                                                << std::endl;

                                        Packet packet;
                                        packet << "hello";

                                        host.send(event.peer, packet);
                                }
                                else if (event.type == Event::Type::Disconnect)
                                {
                                        std::cout << "Peer[id="
                                                << event.peer.id
                                                << "] disconnected"
                                                << std::endl;
                                }
                                else if (event.type == Event::Type::Receive)
                                {
                                        std::string msg;
                                        event.packet >> msg;

                                        std::cout << "Received[id="
                                                << event.peer.id << "]: "
                                                << msg << std::endl;
                                }
                        }
                }
        }
        else
        {
                Host host;

                if (!host.create("localhost", port, 1))
                {
                        std::cout << "Could not create peer." << std::endl;
                        return EXIT_FAILURE;
                }

                std::cout << "Peer created." << std::endl;

                // Try to connect to the host
                if (!host.connect("localhost", SERVER_PORT))
                {
                        std::cout << "Could not connect to the server." << std::endl;
                        return EXIT_FAILURE;
                }

                while (true)
                {
                        Event event;
                        while (host.pollEvent(event))
                        {
                                if (event.type == Event::Type::Connect)
                                {
                                        std::cout << "Connected[id="
                                                << event.peer.id << "]"
                                                << std::endl;

                                        Packet packet(Packet::Flag::Reliable);
                                        packet << "ping";
                                        host.send(event.peer, packet);
                                }
                                else if (event.type == Event::Type::Disconnect)
                                {
                                        std::cout << "Disconnected[id="
                                                << event.peer.id << "]"
                                                << std::endl;
                                }
                                else if (event.type == Event::Type::Receive)
                                {
                                        std::string msg;
                                        event.packet >> msg;

                                        std::cout << "Received[id="
                                                << event.peer.id << "]: "
                                                << msg << std::endl;
                                }
                        }
                }
        }

        enet_deinitialize();
        return EXIT_SUCCESS;
}
