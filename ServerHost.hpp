#ifndef SERVER_HOST_HPP
#define SERVER_HOST_HPP

#include <map>
#include <SFML/Graphics.hpp>
#include "Network.hpp"

struct ServerHost {
        Server server;

        std::map<int, sf::CircleShape> clients;
};

#endif /* Header guards */
