#pragma once


#include "graphics/overlay.hpp"
#include "skyland/scene.hpp"



namespace skyland {



class IntroCreditsScene : public Scene {
public:
    void enter(Platform&, App&, Scene& prev) override;
    void exit(Platform&, App&, Scene& next) override;


    ScenePtr<Scene> update(Platform&, App&, Microseconds delta) override;


private:
    bool wait_ = true;
    std::optional<Text> text_;
    Microseconds timer_ = 0;
};



} // namespace skyland
