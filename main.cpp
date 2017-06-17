#include <iostream>
#include "Network.hpp"

#define PORT 42323

int main(int argc, char** argv)
{
        if(enet_initialize() != 0)
                std::cerr << "Could not initialize enet." << std::endl;

        char mode;
        std::cout << "Server(s) or Client(c)? " << std::flush;
        std::cin >> mode;

        if(mode == 's') {
                Server server(PORT);

                while(true) {
                        Event event;

                        while(server.pollEvent(event)) {
                                if(event.type == Event::CONNECTED) {
                                        std::cout << "New client[id=" << event.peerId <<
                                                "] connected from [" << event.ip <<
                                                ":" << event.port << "]." <<  std::endl;
                                } else if(event.type == Event::RECEIVED) {
                                        std::cout << "Received[id=" << event.peerId <<
                                                "]: " << event.data << std::endl;
                                } else if(event.type == Event::DISCONNECTED) {
                                        std::cout << "Client[id=" << event.peerId <<
                                                "] disconnected." << std::endl;
                                }
                        }
                }
        } else {
                Client client;

                /* Try to connect to the server */
                if(!client.connect("localhost", PORT)) {
                        std::cerr << "Could not connect to the server." << std::endl;
                }

                for(int i = 0; i < 1000000; i++) {
                        Event event;

                        while(client.pollEvent(event)) {
                                if(event.type == Event::CONNECTED) {
                                        std::cout << "Connected to the server[id=" <<
                                                event.peerId << "]." << std::endl;
                                        client.send("ping");
                                }
                        }
                }

                client.disconnect();
        }

        return 0;
}
