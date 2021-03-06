#pragma once


#include "skyland/entity.hpp"



namespace skyland {



class Explosion : public Entity {
public:
    static const int start_index = 19;


    Explosion(const Vec2<Float>& position) : Entity({{}, {}})
    {
        sprite_.set_position(position);
        sprite_.set_size(Sprite::Size::w16_h32);
        sprite_.set_texture_index(start_index);
        sprite_.set_origin({8, 8});
    }


    void update(Platform&, App&, Microseconds delta) override
    {
        timer_ += delta;

        if (timer_ > milliseconds(55)) {
            timer_ = 0;

            auto index = sprite_.get_texture_index();
            if (index < start_index + 5) {
                sprite_.set_texture_index(index + 1);
            } else {
                kill();
            }
        }
    }


private:
    Microseconds timer_ = 0;
};



void medium_explosion(Platform& pfrm, App& app, const Vec2<Float>& position);



void big_explosion(Platform& pfrm, App& app, const Vec2<Float>& position);



} // namespace skyland
