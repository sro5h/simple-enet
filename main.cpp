#include <iostream>
#include "Network.hpp"

#define PORT 42323

int main(int argc, char** argv)
{
        if (enet_initialize() != 0)
                std::cerr << "Could not initialize enet." << std::endl;

        char mode;
        std::cout << "Server(s) or Client(c)? " << std::flush;
        std::cin >> mode;

        if (mode == 's') {
                Server server;

                if (!server.create(PORT, 32)) {
                        std::cerr << "Could not create the host." << std::endl;
                        return 1;
                }

                std::cout << "Host created." << std::endl;

                while (true) {
                        Event event;

                        while (server.pollEvent(event)) {
                                if (event.type == EventType::Connected) {
                                        std::cout << "New client[id=" << event.peerId <<
                                                "] connected from [" << event.ip <<
                                                ":" << event.port << "]." <<  std::endl;

                                } else if (event.type == EventType::Received) {
                                        std::cout << "Received[id=" << event.peerId <<
                                                "]: " << event.data << std::endl;
                                        server.sendToAll("pong");

                                } else if (event.type == EventType::Disconnected) {
                                        std::cout << "Client[id=" << event.peerId <<
                                                "] disconnected." << std::endl;
                                }
                        }
                }
        } else {
                Client client;
                bool connected = true;
                int i = 0;

                if (!client.create(PORT_ANY, 1)) {
                        std::cerr << "Could not create the host." << std::endl;
                        return 1;
                }

                std::cout << "Host created." << std::endl;

                /* Try to connect to the server */
                if (!client.connect("localhost", PORT)) {
                        std::cerr << "Could not connect to the server." << std::endl;
                }

                while (connected) {
                        Event event;

                        while (client.pollEvent(event)) {
                                if (event.type == EventType::Connected) {
                                        std::cout << "Connected to the server[id=" <<
                                                event.peerId << "]." << std::endl;
                                        client.send("ping");

                                } else if (event.type == EventType::Received) {
                                        std::cout << "Received[id=" << event.peerId <<
                                                "]: " << event.data << std::endl;

                                } else if (event.type == EventType::Disconnected) {
                                        std::cout << "Disconnected from the server[id=" <<
                                                event.peerId << "]." << std::endl;
                                        connected = false;
                                }
                        }

                        if (i > 10000000) {
                                client.disconnect();
                        }

                        ++i;
                }
        }

        enet_deinitialize();
        return 0;
}
