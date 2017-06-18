#include <iostream>
#include <SFML/Graphics.hpp>
#include "Network.hpp"

#define PORT 42323

bool runServer(sf::RenderWindow& window);
bool runClient(sf::RenderWindow& window);
void pollServer(Server& server);
void pollClient(Client& client);

int main(int argc, char** argv)
{
        std::string mode = "server";

        if (argc > 1)
                mode = argv[1];

        /* Initialize enet */
        if (enet_initialize() != 0)
                std::cerr << "Could not initialize enet." << std::endl;

        /* Create the window */
        sf::RenderWindow window(sf::VideoMode(400, 400), mode);

        if (mode == "server") {
                if(!runServer(window))
                        return 1;

        } else {
                if(!runClient(window))
                        return 1;
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
                sf::Event event;

                /* Poll the window for events */
                while (window.pollEvent(event)) {
                        if (event.type == sf::Event::Closed) {
                                window.close();
                        }
                        if (event.type == sf::Event::KeyPressed &&
                                        event.key.code == sf::Keyboard::Q) {
                                window.close();
                        }
                }

                /* Poll the server for events */
                pollServer(server);

                window.clear();
                window.display();
        }

        return true;
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
                sf::Event event;

                /* Poll the window for events */
                while (window.pollEvent(event)) {
                        if (event.type == sf::Event::Closed) {
                                window.close();
                        }
                        if (event.type == sf::Event::KeyPressed &&
                                        event.key.code == sf::Keyboard::Q) {
                                window.close();
                        }
                        if (event.type == sf::Event::KeyPressed &&
                                        event.key.code == sf::Keyboard::D) {
                                client.disconnect();
                        }
                }

                /* Poll the server for events */
                pollClient(client);

                window.clear();
                window.display();
        }

        return true;
}

void pollServer(Server& server)
{
        Event event;

        while (server.pollEvent(event)) {
                if (event.type == Event::CONNECTED) {
                        std::cout << "New client[id=" << event.peerId <<
                                "] connected from [" << event.ip <<
                                ":" << event.port << "]." <<  std::endl;
                } else if (event.type == Event::RECEIVED) {
                        std::cout << "Received[id=" << event.peerId <<
                                "]: " << event.data << std::endl;
                } else if (event.type == Event::DISCONNECTED) {
                        std::cout << "Client[id=" << event.peerId <<
                                "] disconnected." << std::endl;
                }
        }
}

void pollClient(Client& client)
{
        Event event;

        while (client.pollEvent(event)) {
                if (event.type == Event::CONNECTED) {
                        std::cout << "Connected to the server[id=" <<
                                event.peerId << "]." << std::endl;
                        client.send("ping");

                } else if (event.type == Event::DISCONNECTED) {
                        std::cout << "Disconnected from the server[id=" <<
                                event.peerId << "]." << std::endl;
                }
        }
}
