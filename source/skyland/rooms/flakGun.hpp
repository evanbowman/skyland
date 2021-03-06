#pragma once


#include "skyland/coins.hpp"
#include "skyland/room.hpp"



namespace skyland {



class FlakGun : public Room {
public:
    FlakGun(Island* parent, const Vec2<u8>& position);


    void update(Platform&, App&, Microseconds delta) override;


    void render_interior(u8 buffer[16][16]) override;


    void render_exterior(u8 buffer[16][16]) override;


    bool has_roof() override
    {
        return false;
    }


    bool description_visible() override
    {
        return true;
    }


    static Vec2<u8> size()
    {
        return {2, 1};
    }


    static const char* name()
    {
        return "flak-gun";
    }


    static Float ai_base_weight()
    {
        return 800.f;
    }


    ScenePtr<Scene> select(Platform& pfrm, App&) override;


    void set_target(const Vec2<u8>& target) override
    {
        target_ = target;
    }


    void unset_target() override
    {
        target_.reset();
    }


    void plot_walkable_zones(bool matrix[16][16]) override
    {
        // characters cannot walk through a flak gun.
    }


    static Conditions::Value conditions()
    {
        return Conditions::workshop_required;
    }


    static Icon icon()
    {
        return 936;
    }


    static Icon unsel_icon()
    {
        return 920;
    }


    Microseconds reload_time_remaining() const override
    {
        return reload_;
    }


private:
    static constexpr const Microseconds reload_time = milliseconds(4500);

    Microseconds reload_ = reload_time;


    std::optional<Vec2<u8>> target_;
};



} // namespace skyland
