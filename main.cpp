#include <iostream>
#include <SFML/Graphics.hpp>
#include "Network.hpp"

#define PORT 42323

bool runServer(sf::RenderWindow& window);
bool runClient(sf::RenderWindow& window);

int main(int argc, char** argv)
{
        std::string mode = "server";
        if (argc > 1) {
                mode = argv[1];
        }

        /* Initialize enet */
        if (enet_initialize() != 0)
                std::cerr << "Could not initialize enet." << std::endl;

        /* Create the window */
        sf::RenderWindow window(sf::VideoMode(400, 400), mode);

        if (mode == "server") {
                if(!runServer(window)) {
                        return 1;
                }
        } else {
                if(!runClient(window)) {
                        return 1;
                }
        }

        return 0;
}

bool runServer(sf::RenderWindow& window)
{
        Server server;

        if (!server.create(PORT)) {
                std::cerr << "Could not create the host." << std::endl;
                return false;
        }

        std::cout << "Host created." << std::endl;

        while (window.isOpen()) {
                sf::Event windowEvent;
                Event hostEvent;

                /* Poll the window for events */
                while (window.pollEvent(windowEvent)) {
                        if (windowEvent.type == sf::Event::Closed) {
                                window.close();
                        }
                        if (windowEvent.type == sf::Event::KeyPressed &&
                                        windowEvent.key.code == sf::Keyboard::Q) {
                                window.close();
                        }
                }

                /* Poll the server for events */
                while (server.pollEvent(hostEvent)) {
                        if (hostEvent.type == Event::CONNECTED) {
                                std::cout << "New client[id=" << hostEvent.peerId <<
                                        "] connected from [" << hostEvent.ip <<
                                        ":" << hostEvent.port << "]." <<  std::endl;
                        } else if (hostEvent.type == Event::RECEIVED) {
                                std::cout << "Received[id=" << hostEvent.peerId <<
                                        "]: " << hostEvent.data << std::endl;
                        } else if (hostEvent.type == Event::DISCONNECTED) {
                                std::cout << "Client[id=" << hostEvent.peerId <<
                                        "] disconnected." << std::endl;
                        }
                }

                window.clear();
                window.display();
        }

        return true;
}

void handleServerEvents(Server& server)
{
}

bool runClient(sf::RenderWindow& window)
{
        Client client;

        if (!client.create(PORT_ANY)) {
                std::cerr << "Could not create the host." << std::endl;
                return false;
        }

        std::cout << "Host created." << std::endl;

        /* Try to connect to the server */
        if (!client.connect("localhost", PORT)) {
                std::cerr << "Could not connect to the server." << std::endl;
        }

        while (window.isOpen()) {
                sf::Event windowEvent;
                Event hostEvent;

                /* Poll the window for events */
                while (window.pollEvent(windowEvent)) {
                        if (windowEvent.type == sf::Event::Closed) {
                                window.close();
                        }
                        if (windowEvent.type == sf::Event::KeyPressed &&
                                        windowEvent.key.code == sf::Keyboard::Q) {
                                window.close();
                        }
                        if (windowEvent.type == sf::Event::KeyPressed &&
                                        windowEvent.key.code == sf::Keyboard::D) {
                                client.disconnect();
                        }
                }

                /* Poll the server for events */
                while (client.pollEvent(hostEvent)) {
                        if (hostEvent.type == Event::CONNECTED) {
                                std::cout << "Connected to the server[id=" <<
                                        hostEvent.peerId << "]." << std::endl;
                                client.send("ping");

                        } else if (hostEvent.type == Event::DISCONNECTED) {
                                std::cout << "Disconnected from the server[id=" <<
                                        hostEvent.peerId << "]." << std::endl;
                        }
                }

                window.clear();
                window.display();
        }

        return true;
}
