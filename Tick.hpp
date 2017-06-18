#ifndef TICK_HPP
#define TICK_HPP

#include "json.hpp"

/* For convenience */
using json = nlohmann::json;

struct Tick {
        float x;
        float y;
};

void to_json(json& j, const Tick& t)
{
        j = json { { "x", t.x }, { "y", t.y } };
}

void from_json(const json& j, Tick& t)
{
        t.x = j.at("x").get<float>();
        t.y = j.at("y").get<float>();
}

#endif /* Header guards */
