#pragma once

#include "skyland/room.hpp"



namespace skyland {



class Core : public Room {
public:
    Core(Island* parent, const Vec2<u8>& position);


    void update(Platform&, App&, Microseconds delta) override;


    void render_interior(Platform& pfrm, Layer layer) override;
    void render_exterior(Platform& pfrm, Layer layer) override;
};



}
