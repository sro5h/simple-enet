#ifndef TICK_HPP
#define TICK_HPP

#include "json.hpp"

/* For convenience */
using json = nlohmann::json;

struct Tick {
        bool left;
        bool right;

        Tick()
        {
                left = false;
                right = false;
        }
};

void to_json(json& j, const Tick& t)
{
        j = json { { "left", t.left }, { "right", t.right } };
}

void from_json(const json& j, Tick& t)
{
        t.left = j.at("left").get<bool>();
        t.right = j.at("right").get<bool>();
}

#endif /* Header guards */
