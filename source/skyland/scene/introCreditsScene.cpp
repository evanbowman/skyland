#include "introCreditsScene.hpp"
#include "skyland/scene_pool.hpp"
#include "newgameScene.hpp"
#include "titleScreenScene.hpp"



namespace skyland {



void IntroCreditsScene::enter(Platform& pfrm, App&, Scene& prev)
{
    pfrm.load_overlay_texture("overlay");

    pfrm.screen().fade(1.f);
}



void IntroCreditsScene::exit(Platform& pfrm, App&, Scene& next)
{
    text_.reset();
    pfrm.screen().fade(0.f);
}



ScenePtr<Scene> IntroCreditsScene::update(Platform& pfrm, App&, Microseconds delta)
{
    timer_ += delta;

    if (wait_) {
        if (timer_ > milliseconds(500)) {
            wait_ = false;
            timer_ = 0;

            text_.emplace(pfrm, "Evan Bowman presents", OverlayCoord{4, 7});
            pfrm.set_overlay_origin(-4, 0);
        }
    } else if (text_) {
        if (timer_ > seconds(4) or pfrm.keyboard().down_transition<Key::action_2>()) {
            text_.reset();
            timer_ = 0;
        }
    } else {
        pfrm.set_overlay_origin(0, 0);
        // if (timer_ > milliseconds(600)) {
            return scene_pool::alloc<TitleScreenScene>();
        // }
    }

    return null_scene();
}




}
