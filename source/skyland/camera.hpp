#pragma once

#include "number/numeric.hpp"



class Platform;



namespace skyland {



class Island;



class Camera {
public:
    void update(Platform& pfrm,
                Island& target,
                const Vec2<u8>& cursor_loc,
                Microseconds delta,
                bool near);


private:
    Vec2<int> target_;
    Vec2<Float> current_;
};



} // namespace skyland
