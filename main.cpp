#include <iostream>
#include "ServerHost.hpp"
#include "Tick.hpp"

#define PORT 42323

bool serverRun(sf::RenderWindow& window);
bool clientRun(sf::RenderWindow& window);
void serverPoll(ServerHost& serverHost);
void clientPoll(Client& client);
void serverOnReceive(ServerHost& serverHost, const Event& event);
void clientHandleInput(Client& client, sf::RenderWindow& window);

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
                if(!serverRun(window))
                        return 1;

        } else {
                if(!clientRun(window))
                        return 1;
        }

        return 0;
}

bool serverRun(sf::RenderWindow& window)
{
        ServerHost serverHost;

        if (!serverHost.server.create(PORT)) {
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
                serverPoll(serverHost);

                window.clear();

                /* Print all clients */
                for(auto& entry : serverHost.clients) {
                        window.draw(entry.second);
                }

                window.display();
        }

        return true;
}

bool clientRun(sf::RenderWindow& window)
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
                                        event.key.code == sf::Keyboard::C) {
                                client.disconnect();
                        }
                }

                /* Handle keyboard input */
                clientHandleInput(client, window);

                /* Poll the server for events */
                clientPoll(client);

                window.clear();
                window.display();
        }

        return true;
}

void serverPoll(ServerHost& serverHost)
{
        Event event;

        while (serverHost.server.pollEvent(event)) {
                if (event.type == Event::CONNECTED) {
                        std::cout << "New client[id=" << event.peerId <<
                                "] connected from [" << event.ip <<
                                ":" << event.port << "]." <<  std::endl;
                        serverHost.clients[event.peerId] = sf::CircleShape(10.0f);

                } else if (event.type == Event::RECEIVED) {
                        std::cout << "Received[id=" << event.peerId <<
                                "]: " << event.data << std::endl;
                        serverOnReceive(serverHost, event);

                } else if (event.type == Event::DISCONNECTED) {
                        std::cout << "Client[id=" << event.peerId <<
                                "] disconnected." << std::endl;
                        serverHost.clients.erase(event.peerId);
                        std::cout << "Shape count: " <<
                                serverHost.clients.size() << std::endl;
                }
        }
}

void clientPoll(Client& client)
{
        Event event;

        while (client.pollEvent(event)) {
                if (event.type == Event::CONNECTED) {
                        std::cout << "Connected to the server[id=" <<
                                event.peerId << "]." << std::endl;

                } else if (event.type == Event::RECEIVED) {
                        std::cout << "Received[id=" << event.peerId <<
                                "]: " << event.data << std::endl;

                } else if (event.type == Event::DISCONNECTED) {
                        std::cout << "Disconnected from the server[id=" <<
                                event.peerId << "]." << std::endl;
                }
        }
}

void serverOnReceive(ServerHost& serverHost, const Event& event)
{
        ClientTick t = json::parse(event.data);
        sf::Vector2f tmp = serverHost.clients[event.peerId].getPosition();

        if (t.left) {
                tmp.x -= 0.05;
        }
        if (t.right) {
                tmp.x += 0.05;
        }
        if (t.up) {
                tmp.y -= 0.05;
        }
        if (t.down) {
                tmp.y += 0.05;
        }

        serverHost.clients[event.peerId].setPosition(tmp);
}

void clientHandleInput(Client& client, sf::RenderWindow& window)
{
        ClientTick t;

        if (window.hasFocus()) {
                t.left  = sf::Keyboard::isKeyPressed(sf::Keyboard::A);
                t.right = sf::Keyboard::isKeyPressed(sf::Keyboard::D);
                t.up    = sf::Keyboard::isKeyPressed(sf::Keyboard::W);
                t.down  = sf::Keyboard::isKeyPressed(sf::Keyboard::S);
        }

        json j = t;
        client.send(j.dump());
}
