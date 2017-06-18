#ifndef TICK_HPP
#define TICK_HPP

#include "json.hpp"

/* For convenience */
using json = nlohmann::json;

struct ClientTick {
        bool left;
        bool right;
        bool up;
        bool down;

        ClientTick()
        {
                left = false;
                right = false;
                up = false;
                down = false;
        }
};

void to_json(json& j, const ClientTick& t)
{
        j = json { { "left", t.left }, { "right", t.right }, { "up", t.up }, { "down", t.down } };
}

void from_json(const json& j, ClientTick& t)
{
        t.left = j.at("left").get<bool>();
        t.right = j.at("right").get<bool>();
        t.up = j.at("up").get<bool>();
        t.down = j.at("down").get<bool>();
}

#endif /* Header guards */
