#pragma once

#include "island.hpp"
#include "platform/platform.hpp"
#include "scene.hpp"
#include "coins.hpp"



namespace skyland {



class App {
public:
    App(Platform& pfrm);

    void update(Platform& pfrm, Microseconds delta);
    void render(Platform& pfrm);


    Island& player_island()
    {
        return player_island_;
    }


    void updateParallax(Microseconds delta);


    Coins& coins()
    {
        return coins_;
    }


    Coins& terrain_cost()
    {
        return terrain_cost_;
    }


private:
    Island player_island_;
    Float cloud_scroll_1_;
    Float cloud_scroll_2_;
    ScenePtr<Scene> current_scene_;
    ScenePtr<Scene> next_scene_;
    Coins coins_ = 0;
    Coins terrain_cost_ = 0;
};



} // namespace skyland
