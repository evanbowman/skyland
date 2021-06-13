#pragma once


#include "graphics/overlay.hpp"
#include "skyland/coins.hpp"
#include "skyland/scene.hpp"



namespace skyland {



class WorldScene : public Scene {
public:
    ScenePtr<Scene> update(Platform&, App&, Microseconds delta) override;


    void enter(Platform&, App&, Scene& prev) override;


    void exit(Platform&, App&, Scene& next) override;


    void persist_coins();


private:
    std::optional<UIMetric> coins_;
    bool persistent_coins_ = false;
    Microseconds coin_hide_timer_ = 0;
    Coins last_coins_ = 0;
};



} // namespace skyland
