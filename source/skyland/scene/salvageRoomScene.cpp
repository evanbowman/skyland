#include "salvageRoomScene.hpp"
#include "globals.hpp"
#include "localization.hpp"
#include "readyScene.hpp"
#include "skyland/network.hpp"
#include "skyland/room_metatable.hpp"
#include "skyland/skyland.hpp"



namespace skyland {



static Coins salvage_value(Room& room)
{
    return ((*room.metaclass())->cost() * salvage_factor) *
           (Float(room.health()) / (*room.metaclass())->full_health());
}



void SalvageRoomScene::enter(Platform& pfrm, App& app, Scene& prev)
{
    WorldScene::enter(pfrm, app, prev);

    auto st = calc_screen_tiles(pfrm);
    StringBuffer<30> text("really salvage?  +");

    auto& cursor_loc = std::get<SkylandGlobalData>(globals()).near_cursor_loc_;
    if (auto room = app.player_island().get_room(cursor_loc)) {
        if (auto mt = room->metaclass()) {
            if (str_cmp((*mt)->name(), "power-core") == 0) {
                int core_count = 0;
                for (auto& room : app.player_island().rooms()) {
                    if (str_cmp((*room->metaclass())->name(), "power-core") ==
                        0) {
                        core_count++;
                    }
                }
                if (core_count == 1) {
                    // That would be suicide! You can't salvage your island's
                    // only power core.
                    exit_countdown_ = 1;
                }
            }
            text += to_string<10>(salvage_value(*room));
        } else {
            text += "0";
        }
        if (length(room->characters()) > 0) {
            exit_countdown_ = 1;
        }
    }

    text += "@";

    text_.emplace(pfrm, text.c_str(), OverlayCoord{0, u8(st.y - 1)});

    const int count = st.x - text_->len();
    for (int i = 0; i < count; ++i) {
        pfrm.set_tile(Layer::overlay, i + text_->len(), st.y - 1, 426);
    }

    for (int i = 0; i < st.x; ++i) {
        pfrm.set_tile(Layer::overlay, i, st.y - 2, 425);
    }

    yes_text_.emplace(pfrm, OverlayCoord{u8(st.x - 7), u8(st.y - 3)});
    no_text_.emplace(pfrm, OverlayCoord{u8(st.x - 7), u8(st.y - 2)});

    yes_text_->assign("A: yes ");
    no_text_->assign("B:  no ");

    for (int i = 23; i < st.x; ++i) {
        pfrm.set_tile(Layer::overlay, i, st.y - 4, 425);
    }

    pfrm.set_tile(Layer::overlay, st.x - 8, st.y - 2, 419);
    pfrm.set_tile(Layer::overlay, st.x - 8, st.y - 3, 128);

    persist_ui();
}



void SalvageRoomScene::exit(Platform& pfrm, App& app, Scene& next)
{
    WorldScene::exit(pfrm, app, next);

    text_.reset();
    yes_text_.reset();
    no_text_.reset();

    const auto st = calc_screen_tiles(pfrm);
    for (int x = 0; x < st.x; ++x) {
        pfrm.set_tile(Layer::overlay, x, st.y - 1, 0);
        pfrm.set_tile(Layer::overlay, x, st.y - 2, 0);
        pfrm.set_tile(Layer::overlay, x, st.y - 3, 0);
        pfrm.set_tile(Layer::overlay, x, st.y - 4, 0);
    }
}



ScenePtr<Scene>
SalvageRoomScene::update(Platform& pfrm, App& app, Microseconds delta)
{
    if (auto next = ActiveWorldScene::update(pfrm, app, delta)) {
        return next;
    }

    if (exit_countdown_) {
        exit_countdown_ -= delta;
        if (exit_countdown_ <= 0) {
            return scene_pool::alloc<ReadyScene>();
        }
    } else {
        auto& cursor_loc =
            std::get<SkylandGlobalData>(globals()).near_cursor_loc_;

        if (app.player().key_down(pfrm, Key::action_1)) {
            if (auto room = app.player_island().get_room(cursor_loc)) {

                // You cannot salvage an occupied room, doing so would destroy
                // all of the characters inside.
                if (length(room->characters()) == 0) {

                    app.coins() += salvage_value(*room);

                    app.player_island().destroy_room(pfrm, cursor_loc);
                    exit_countdown_ = milliseconds(500);

                    network::packet::RoomSalvaged packet;
                    packet.x_ = cursor_loc.x;
                    packet.y_ = cursor_loc.y;
                    network::transmit(pfrm, packet);
                }
            } else {
                return scene_pool::alloc<ReadyScene>();
            }
        }
    }



    if (app.player().key_down(pfrm, Key::action_2)) {
        return scene_pool::alloc<ReadyScene>();
    }

    return null_scene();
}



} // namespace skyland
