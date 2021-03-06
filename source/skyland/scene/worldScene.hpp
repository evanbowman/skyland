#pragma once


#include "graphics/overlay.hpp"
#include "skyland/coins.hpp"
#include "skyland/gamespeed.hpp"
#include "skyland/power.hpp"
#include "skyland/scene.hpp"



namespace skyland {



class WorldScene : public Scene {
public:
    ScenePtr<Scene> update(Platform&, App&, Microseconds delta) override;


    void display(Platform&, App&) override;


    void enter(Platform&, App&, Scene& prev) override;


    void exit(Platform&, App&, Scene& next) override;


    void persist_ui();


    void far_camera();


    void near_camera();


protected:
    std::optional<UIMetric> coins_;
    std::optional<UIMetric> power_;

    Microseconds camera_update_timer_ = 0;

    void reset_gamespeed(Platform& pfrm, App& app);
    void set_gamespeed(Platform& pfrm, App& app, GameSpeed speed);

private:
    void set_pause_icon(Platform& pfrm, u16 icon);

    bool persistent_ui_ = false;
    Microseconds coin_hide_timer_ = 0;
    Microseconds power_hide_timer_ = 0;
    Microseconds set_gamespeed_keyheld_timer_ = 0;
    Coins last_coins_ = 0;
    Power last_power_supplied_ = 0;
    Power last_power_used_ = 0;
    bool far_camera_ = false;
};



class ActiveWorldScene : public WorldScene {
public:
    ScenePtr<Scene> update(Platform&, App&, Microseconds delta) override;
};



} // namespace skyland
