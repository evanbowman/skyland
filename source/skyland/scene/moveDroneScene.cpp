#include "moveDroneScene.hpp"
#include "readyScene.hpp"
#include "skyland/rooms/droneBay.hpp"
#include "skyland/skyland.hpp"



namespace skyland {



MoveDroneScene::MoveDroneScene(Platform& pfrm, Vec2<u8> origin, bool near)
    : matrix_(allocate_dynamic<bool[16][16]>(pfrm)), origin_(origin),
      near_(near)
{
    if (not matrix_) {
        pfrm.fatal("MDS: buffers exhausted");
    }

    for (int x = 0; x < 16; ++x) {
        for (int y = 0; y < 16; ++y) {
            (*matrix_)[x][y] = true;
        }
    }

    camera_update_timer_ = milliseconds(500);
}



void MoveDroneScene::enter(Platform& pfrm, App& app, Scene& prev)
{
    ActiveWorldScene::enter(pfrm, app, prev);

    if (not near_) {
        far_camera();
    }

    message_.emplace(pfrm,
                     "launch drone: select position",
                     OverlayCoord{0, 19});

    for (int i = 0; i < message_->len(); ++i) {
        pfrm.set_tile(Layer::overlay, i, 18, 425);
    }

    Island* island = &app.player_island();
    if (not near_ and app.opponent_island()) {
        island = &*app.opponent_island();
    }

    for (auto& room : island->rooms()) {
        auto pos = room->position();
        for (int x = 0; x < room->size().x; ++x) {
            for (int y = 0; y < room->size().y; ++y) {
                (*matrix_)[pos.x + x][pos.y + y] = false;
            }
        }
        if (room->has_roof()) {
            for (int x = 0; x < room->size().x; ++x) {
                (*matrix_)[pos.x + x][pos.y - 1] = false;
            }
        }
    }

    for (int x = 0; x < 16; ++x) {
        for (int y = 0; y < 16; ++y) {
            if (x >= (int)island->terrain().size()) {
                (*matrix_)[x][y] = false;
            }
            if (y > 14) {
                (*matrix_)[x][y] = false;
            }
            if ((*matrix_)[x][y] == true) {
                pfrm.set_tile(island->layer(), x, y, 103);
            }
        }
    }
}



void MoveDroneScene::exit(Platform& pfrm, App& app, Scene& next)
{
    ActiveWorldScene::exit(pfrm, app, next);

    message_.reset();
    pfrm.fill_overlay(0);

    Island* island = &app.player_island();
    if (not near_ and app.opponent_island()) {
        island = &*app.opponent_island();
    }

    for (int x = 0; x < 16; ++x) {
        for (int y = 0; y < 15; ++y) {
            pfrm.set_tile(island->layer(), x, y, 0);
        }
    }

    island->repaint(pfrm);
}



void MoveDroneScene::display(Platform& pfrm, App& app)
{
    ActiveWorldScene::display(pfrm, app);

    Sprite cursor;
    cursor.set_size(Sprite::Size::w16_h32);
    cursor.set_texture_index(15 + cursor_anim_frame_);

    Island* island = &app.player_island();
    if (not near_ and app.opponent_island()) {
        island = &*app.opponent_island();
    }

    Vec2<Float> origin = island->origin();

    Vec2<u8> cursor_loc;
    if (near_) {
        cursor_loc = std::get<SkylandGlobalData>(globals()).near_cursor_loc_;
    } else {
        cursor_loc = std::get<SkylandGlobalData>(globals()).far_cursor_loc_;
    }

    origin.x += cursor_loc.x * 16;
    origin.y += cursor_loc.y * 16;

    cursor.set_position(origin);

    pfrm.screen().draw(cursor);
}



ScenePtr<Scene>
MoveDroneScene::update(Platform& pfrm, App& app, Microseconds delta)
{
    if (auto new_scene = WorldScene::update(pfrm, app, delta)) {
        return new_scene;
    }

    if (app.player().key_down(pfrm, Key::action_2)) {
        return scene_pool::alloc<ReadyScene>();
    }

    cursor_anim_timer_ += delta;
    if (cursor_anim_timer_ > milliseconds(200)) {
        cursor_anim_timer_ -= milliseconds(200);
        cursor_anim_frame_ = not cursor_anim_frame_;
    }

    Island* island = &app.player_island();
    if (not near_ and app.opponent_island()) {
        island = &*app.opponent_island();
    }

    Vec2<u8>* cursor_loc;
    if (near_) {
        cursor_loc = &std::get<SkylandGlobalData>(globals()).near_cursor_loc_;
    } else {
        cursor_loc = &std::get<SkylandGlobalData>(globals()).far_cursor_loc_;
    }

    if (app.player().key_down(pfrm, Key::action_1)) {
        if ((*matrix_)[cursor_loc->x][cursor_loc->y]) {
            if (auto room = app.player_island().get_room(origin_)) {
                if (auto db = dynamic_cast<DroneBay*>(room)) {
                    if (auto drone = alloc_shared_entity<Drone>(
                            room->parent(),
                            island,
                            Vec2<u8>{origin_.x, u8(origin_.y - 1)})) {
                        (*drone)->set_movement_target(*cursor_loc);
                        db->attach_drone(*drone);
                        island->drones().push(*drone);

                        return scene_pool::alloc<ReadyScene>();
                    }
                }
            }
        }
    }

    if (app.player().key_down(pfrm, Key::left)) {
        if (cursor_loc->x > 0) {
            --cursor_loc->x;
        } else if (not near_) {
            std::get<SkylandGlobalData>(globals()).near_cursor_loc_.y = cursor_loc->y;
            std::get<SkylandGlobalData>(globals()).near_cursor_loc_.x =
                app.player_island().terrain().size() - 1;
            return scene_pool::alloc<MoveDroneScene>(pfrm, origin_, true);
        }
    }

    if (app.player().key_down(pfrm, Key::right)) {
        if (cursor_loc->x < island->terrain().size() - 1) {
            ++cursor_loc->x;
        } else if (near_) {
            std::get<SkylandGlobalData>(globals()).far_cursor_loc_.y = cursor_loc->y;
            std::get<SkylandGlobalData>(globals()).far_cursor_loc_.x = 0;
            return scene_pool::alloc<MoveDroneScene>(pfrm, origin_, false);
        }
    }

    if (app.player().key_down(pfrm, Key::up)) {
        if (cursor_loc->y > 6) {
            --cursor_loc->y;
        }
    }

    if (app.player().key_down(pfrm, Key::down)) {
        if (cursor_loc->y < 14) {
            ++cursor_loc->y;
        }
    }


    return null_scene();
}


} // namespace skyland
