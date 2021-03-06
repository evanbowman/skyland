#include "repairDroneRangeScene.hpp"
#include "readyScene.hpp"
#include "skyland/island.hpp"
#include "skyland/skyland.hpp"



namespace skyland {



ScenePtr<Scene>
RepairDroneRangeScene::update(Platform& pfrm, App& app, Microseconds delta)
{
    if (auto scene = ActiveWorldScene::update(pfrm, app, delta)) {
        return scene;
    }

    if (not description_) {
        description_.emplace(
            pfrm, "showing: repair range", OverlayCoord{0, 19});

        for (int i = 0; i < description_->len(); ++i) {
            pfrm.set_tile(Layer::overlay, i, 18, 425);
        }
    }

    if (not drone_.promote() or app.player().key_down(pfrm, Key::action_2)) {
        description_.reset();
        pfrm.fill_overlay(0);
        return scene_pool::alloc<ReadyScene>();
    }

    return null_scene();
}



void RepairDroneRangeScene::display(Platform& pfrm, App& app)
{
    WorldScene::display(pfrm, app);

    if (auto drone_sp = drone_.promote()) {
        auto pos = (*drone_sp)->position();

        auto origin = (*drone_sp)->destination()->origin();

        Sprite sprite;
        sprite.set_size(Sprite::Size::w16_h32);
        sprite.set_texture_index(13);

        for (int x = pos.x - 2; x < pos.x + 3; ++x) {
            sprite.set_position(
                {origin.x + x * 16, origin.y + (pos.y - 2) * 16});
            pfrm.screen().draw(sprite);


            if (x not_eq pos.x) {
                sprite.set_texture_index(14);

                sprite.set_position(
                    {origin.x + x * 16, origin.y + (pos.y - 1) * 16});

                pfrm.screen().draw(sprite);

                sprite.set_texture_index(13);
            }


            sprite.set_position(
                {origin.x + x * 16, origin.y + (pos.y + 1) * 16});

            pfrm.screen().draw(sprite);
        }
    }
}



} // namespace skyland
