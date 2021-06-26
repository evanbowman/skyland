#include "recoverCharacterScene.hpp"
#include "globals.hpp"
#include "readyScene.hpp"
#include "skyland/scene_pool.hpp"
#include "skyland/skyland.hpp"



namespace skyland {



RecoverCharacterScene::RecoverCharacterScene(const Vec2<u8>& transporter_loc)
    : transporter_loc_(transporter_loc)
{
}



ScenePtr<Scene>
RecoverCharacterScene::update(Platform& pfrm, App& app, Microseconds delta)
{
    if (auto new_scene = ActiveWorldScene::update(pfrm, app, delta)) {
        return new_scene;
    }

    // if (targets_.empty()) {
    //     return scene_pool::alloc<ReadyScene>();
    // }

    [[maybe_unused]]
    auto& cursor_loc = std::get<SkylandGlobalData>(globals()).far_cursor_loc_;
    // cursor_loc.x = targets_[selector_].x;
    // cursor_loc.y = targets_[selector_].y;

    if (pfrm.keyboard().down_transition<Key::right>() or
        pfrm.keyboard().down_transition<Key::down>()) {

        // if (selector_ < (int)targets_.size() - 1) {
        //     selector_++;
        // } else {
        //     selector_ = 0;
        // }
    }

    if (pfrm.keyboard().down_transition<Key::left>() or
        pfrm.keyboard().down_transition<Key::up>()) {

        // if (selector_ > 0) {
        //     --selector_;
        // } else {
        //     selector_ = targets_.size() - 1;
        // }
    }

    if (pfrm.keyboard().down_transition<Key::action_1>()) {
        // const auto target = targets_[selector_];
        // if (auto room = app.player_island().get_room(weapon_loc_)) {
        //     room->set_target(target);
        // }
        return scene_pool::alloc<ReadyScene>();
    }

    if (pfrm.keyboard().down_transition<Key::action_2>()) {
        return scene_pool::alloc<ReadyScene>();
    }


    return null_scene();
}


void RecoverCharacterScene::display(Platform& pfrm, App& app)
{
    WorldScene::display(pfrm, app);

    if (not app.opponent_island()) {
        return;
    }

    auto origin = app.opponent_island()->origin();
    // origin.x += targets_[selector_].x * 16;
    // origin.y += targets_[selector_].y * 16;

    Sprite sprite;
    sprite.set_position(origin);
    sprite.set_texture_index(17);
    sprite.set_size(Sprite::Size::w16_h32);

    pfrm.screen().draw(sprite);
}



void RecoverCharacterScene::enter(Platform& pfrm, App& app, Scene& prev)
{
    WorldScene::enter(pfrm, app, prev);

    far_camera();
}



} // namespace skyland
