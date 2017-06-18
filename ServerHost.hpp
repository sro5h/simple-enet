#ifndef SERVER_HOST_HPP
#define SERVER_HOST_HPP

#include <SFML/Graphics.hpp>
#include "Network.hpp"

struct ServerHost {
        Server server;
        sf::CircleShape client;
};

#endif /* Header guards */
