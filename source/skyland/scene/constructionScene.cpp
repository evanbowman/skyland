#include "constructionScene.hpp"
#include "globals.hpp"
#include "localization.hpp"
#include "platform/platform.hpp"
#include "readyScene.hpp"
#include "skyland/room_metatable.hpp"
#include "skyland/scene_pool.hpp"
#include "skyland/skyland.hpp"
#include "skyland/tile.hpp"
#include "worldScene.hpp"



namespace skyland {



int construction_zone_min_y = 7;



static Coins get_cost(App& app, const RoomMeta& meta)
{
    Coins cost = meta->cost();
    for (int i = 0; i < app.player_island().workshop_count(); ++i) {
        cost *= 0.9f;
    }
    return cost;
}



ScenePtr<Scene>
ConstructionScene::update(Platform& pfrm, App& app, Microseconds delta)
{
    if (pfrm.keyboard().down_transition<Key::alt_2>() or
        (state_ == State::select_loc and
         pfrm.keyboard().down_transition<Key::action_2>())) {
        auto& cursor_loc =
            std::get<SkylandGlobalData>(globals()).near_cursor_loc_;
        if (not construction_sites_.empty()) {
            cursor_loc.x = construction_sites_[selector_].x;
            cursor_loc.y = construction_sites_[selector_].y;
        }
        return scene_pool::alloc<ReadyScene>();
    }

    if (auto new_scene = ActiveWorldScene::update(pfrm, app, delta)) {
        return new_scene;
    }

    switch (state_) {
    case State::select_loc:
        if (pfrm.keyboard().down_transition<Key::right>() and
            selector_ < construction_sites_.size() - 1) {
            ++selector_;
        }

        if (pfrm.keyboard().down_transition<Key::left>() and selector_ > 0) {
            --selector_;
        }

        if (not construction_sites_.empty()) {
            auto& cursor_loc =
                std::get<SkylandGlobalData>(globals()).near_cursor_loc_;
            cursor_loc.x = construction_sites_[selector_].x;
            cursor_loc.y = construction_sites_[selector_].y;
        }

        if (pfrm.keyboard().down_transition<Key::action_1>() and
            not construction_sites_.empty()) {

            if (construction_sites_[selector_].y == 15) {
                // Special case: we want to add to the terrain level, not
                // construct a building.
                state_ = State::add_terrain;
                msg(pfrm, ":build :add-terrain");

            } else {
                collect_available_buildings(pfrm, app);

                if (not available_buildings_.empty()) {
                    state_ = State::choose_building;
                    show_current_building_text(pfrm, app);
                }
            }
        }
        break;

    case State::choose_building:
        if (pfrm.keyboard().down_transition<Key::action_2>()) {
            find_construction_sites(pfrm, app);
            state_ = State::select_loc;
            msg(pfrm, ":build");
            break;
        }

        if (pfrm.keyboard().down_transition<Key::up>()) {
            if (building_selector_ < (int)available_buildings_.size() - 1) {
                ++building_selector_;
                show_current_building_text(pfrm, app);
            } else {
                building_selector_ = 0;
                show_current_building_text(pfrm, app);
            }
        }

        if (pfrm.keyboard().down_transition<Key::down>()) {
            if (building_selector_ > 0) {
                --building_selector_;
                show_current_building_text(pfrm, app);
            } else {
                building_selector_ = available_buildings_.size() - 1;
                show_current_building_text(pfrm, app);
            }
        }

        if (pfrm.keyboard().down_transition<Key::action_1>()) {
            const auto& target = *available_buildings_[building_selector_];

            if (app.coins() < get_cost(app, target)) {
                msg(pfrm, "insufficent funds!");
                state_ = State::insufficent_funds;
                break;
            }

            if (app.player_island().power_supply() -
                    app.player_island().power_drain() <
                target->consumes_power()) {
                msg(pfrm, "insufficient power supply!");
                state_ = State::insufficent_funds;
                break;
            }

            app.coins() -= get_cost(app, target);

            const auto sz = target->size().y;
            target->create(pfrm,
                           &app.player_island(),
                           {construction_sites_[selector_].x,
                            u8(construction_sites_[selector_].y - (sz - 1))});

            find_construction_sites(pfrm, app);

            state_ = State::select_loc;
            msg(pfrm, ":build");
        }
        break;

    case State::insufficent_funds:
        if (pfrm.keyboard().down_transition<Key::action_2, Key::action_1>()) {
            find_construction_sites(pfrm, app);
            state_ = State::select_loc;
            msg(pfrm, ":build");
        }
        break;

    case State::add_terrain:
        if (pfrm.keyboard().down_transition<Key::action_2>()) {
            find_construction_sites(pfrm, app);
            state_ = State::select_loc;
            msg(pfrm, ":build");
            break;
        }

        if (pfrm.keyboard().down_transition<Key::action_1>()) {
            if (app.coins() < app.terrain_cost()) {
                msg(pfrm, "insufficent funds!");
                state_ = State::insufficent_funds;
                break;
            }

            app.coins() -= app.terrain_cost();

            auto& terrain = app.player_island().terrain();
            terrain.pop_back(); // the old edge tile
            terrain.push_back(Tile::terrain_middle);
            terrain.push_back(Tile::terrain_right);

            app.player_island().render_terrain(pfrm);

            find_construction_sites(pfrm, app);
            state_ = State::select_loc;

            msg(pfrm, ":build");
        }
        break;
    }

    return null_scene();
}



void ConstructionScene::show_current_building_text(Platform& pfrm, App& app)
{
    StringBuffer<32> str = ":build :";

    str += (*available_buildings_[building_selector_])->name();
    str += " ";
    str += to_string<10>(
        get_cost(app, (*available_buildings_[building_selector_])));
    str += "$";
    str += " ";
    str += to_string<10>(
        (*available_buildings_[building_selector_])->consumes_power());
    str += "`";

    msg(pfrm, str.c_str());
}



void ConstructionScene::display(Platform& pfrm, App& app)
{
    WorldScene::display(pfrm, app);

    switch (state_) {
    case State::insufficent_funds:
        break;

    case State::select_loc:
        if (not construction_sites_.empty()) {
            auto origin = app.player_island().origin();

            origin.x += construction_sites_[selector_].x * 16;
            origin.y += (construction_sites_[selector_].y) * 16;

            Sprite sprite;
            sprite.set_position(origin);
            sprite.set_texture_index(12);
            sprite.set_size(Sprite::Size::w16_h32);


            pfrm.screen().draw(sprite);
        }
        break;


    case State::choose_building:
        if (not available_buildings_.empty()) {
            const auto& meta = *available_buildings_[building_selector_];
            const auto sz = meta->size();

            auto origin = app.player_island().origin();
            origin.x += construction_sites_[selector_].x * 16;
            origin.y += (construction_sites_[selector_].y - (sz.y - 1)) * 16;

            if (sz.x == 1 and sz.y == 1) {
                Sprite sprite;
                sprite.set_texture_index(14);
                sprite.set_size(Sprite::Size::w16_h32);
                sprite.set_position({origin.x, origin.y - 16});
                pfrm.screen().draw(sprite);
            } else {
                Sprite sprite;
                sprite.set_texture_index(13);
                sprite.set_size(Sprite::Size::w16_h32);

                for (int x = 0; x < sz.x; ++x) {
                    for (int y = 0; y < sz.y / 2; ++y) {
                        sprite.set_position(
                            {origin.x + x * 16, origin.y + y * 32});
                        pfrm.screen().draw(sprite);
                    }
                }
            }
        }
        break;


    case State::add_terrain: {
        auto& terrain = app.player_island().terrain();
        const Vec2<u8> loc = {u8(terrain.size()), 15};
        auto origin = app.player_island().origin();
        origin.x += loc.x * 16;
        origin.y -= 32;
        Sprite sprite;
        sprite.set_texture_index(14);
        sprite.set_size(Sprite::Size::w16_h32);
        sprite.set_position(origin);
        pfrm.screen().draw(sprite);
        break;
    }
    }
}



void ConstructionScene::find_construction_sites(Platform& pfrm, App& app)
{
    construction_sites_.clear();

    bool matrix[16][16];

    app.player_island().plot_construction_zones(matrix);

    for (u8 x = 0; x < 16; ++x) {
        for (u8 y = 0; y < 16; ++y) {
            if (matrix[x][y] and y > construction_zone_min_y) {
                construction_sites_.push_back({x, y});
            }
        }
    }

    auto& terrain = app.player_island().terrain();
    if (not terrain.full()) {
        construction_sites_.push_back({u8(terrain.size()), 15});
    }

    if (construction_sites_.empty()) {
        selector_ = 0;
    } else if (selector_ >= construction_sites_.size()) {
        selector_--;
    }
}



void ConstructionScene::msg(Platform& pfrm, const char* text)
{
    auto st = calc_screen_tiles(pfrm);
    text_.emplace(pfrm, text, OverlayCoord{0, u8(st.y - 1)});

    const int count = st.x - text_->len();
    for (int i = 0; i < count; ++i) {
        pfrm.set_tile(Layer::overlay, i + text_->len(), st.y - 1, 426);
    }

    for (int i = 0; i < st.x; ++i) {
        pfrm.set_tile(Layer::overlay, i, st.y - 2, 425);
    }
}



void ConstructionScene::collect_available_buildings(Platform& pfrm, App& app)
{
    available_buildings_.clear();

    int avail_space = 1;

    const auto current = construction_sites_[selector_];
    for (auto& site : construction_sites_) {
        if (site.y == current.y and site.x == current.x + 1) {
            // FIXME: buildings wider than 2, various other cases
            avail_space = 2;
        }
    }

    const int avail_y_space = current.y - construction_zone_min_y;

    const auto w_count = app.player_island().workshop_count();

    auto metatable = room_metatable();
    for (int i = 0; i < metatable.second; ++i) {
        auto& meta = metatable.first[i];

        const bool workshop_required =
            (meta->conditions() & Conditions::workshop_required);

        if (meta->size().x <= avail_space and
            meta->size().y <= avail_y_space and
            (not workshop_required or (workshop_required and w_count > 0)) and
            not (meta->conditions() & Conditions::not_constructible)) {
            available_buildings_.push_back(&meta);
        }
    }

    if (building_selector_ >= (int)available_buildings_.size()) {
        building_selector_ = available_buildings_.size() - 1;
    }
}



void ConstructionScene::enter(Platform& pfrm, App& app, Scene& prev)
{
    WorldScene::enter(pfrm, app, prev);

    persist_ui();

    find_construction_sites(pfrm, app);

    if (not construction_sites_.empty()) {
        auto& cursor_loc =
            std::get<SkylandGlobalData>(globals()).near_cursor_loc_;

        for (u32 i = 0; i < construction_sites_.size(); ++i) {
            if (construction_sites_[i].x == cursor_loc.x) {

                selector_ = i;
            }
        }
    }

    msg(pfrm, ":build");
}



void ConstructionScene::exit(Platform& pfrm, App& app, Scene& next)
{
    WorldScene::exit(pfrm, app, next);

    text_.reset();
    pfrm.fill_overlay(0);
}



} // namespace skyland
