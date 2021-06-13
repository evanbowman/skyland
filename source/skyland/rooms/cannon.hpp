#pragma once


#include "skyland/coins.hpp"
#include "skyland/room.hpp"



namespace skyland {



class Cannon : public Room {
public:
    Cannon(Island* parent, const Vec2<u8>& position);


    void update(Platform&, App&, Microseconds delta) override;


    void render_interior(Platform& pfrm, Layer layer) override;
    void render_exterior(Platform& pfrm, Layer layer) override;


    bool has_roof() override
    {
        return false;
    }


    static Vec2<u8> size()
    {
        return {1, 1};
    }


    static const char* name()
    {
        return "cannon";
    }


    static Coins cost()
    {
        return 800;
    }


    ScenePtr<Scene> select() override;
};


} // namespace skyland
