#pragma once


#include "skyland/player.hpp"



class Platform;



namespace skyland {



class App;



class Opponent : public Player {
public:
    virtual ~Opponent()
    {
    }
};



} // namespace skyland
