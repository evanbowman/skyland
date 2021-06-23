#include "enemyAI.hpp"
#include "number/random.hpp"
#include "skyland/entity/projectile/missile.hpp"
#include "skyland/room_metatable.hpp"
#include "skyland/rooms/cannon.hpp"
#include "skyland/rooms/core.hpp"
#include "skyland/rooms/missileSilo.hpp"
#include "skyland/skyland.hpp"



namespace skyland {



void EnemyAI::update(Platform& pfrm, App& app, Microseconds delta)
{
    if (app.player_island().is_destroyed()) {
        return;
    }

    next_action_timer_ -= delta;
    character_reassign_timer_ -= delta;

    if (next_action_timer_ <= 0) {
        next_action_timer_ = next_action_timeout;

        u8 matrix[16][16];
        app.player_island().plot_rooms(matrix);

        if (app.opponent_island()) {
            for (auto& room : app.opponent_island()->rooms()) {
                if (auto cannon = dynamic_cast<Cannon*>(&*room)) {
                    set_target(pfrm, app, matrix, *cannon);
                } else if (auto silo = dynamic_cast<MissileSilo*>(&*room)) {
                    set_target(pfrm, app, matrix, *silo);
                }
            }
        }
    }

    if (character_reassign_timer_ <= 0) {
        character_reassign_timer_ = character_reassign_timeout;

        for (auto& room : app.player_island().rooms()) {
            for (auto& character : room->characters()) {
                if (character->owner() == this) {
                    assign_boarded_character(pfrm, app, *character);
                }
            }
        }
    }
}



u32 flood_fill(Platform& pfrm, u8 matrix[16][16], u8 replace, u8 x, u8 y);



void EnemyAI::assign_boarded_character(Platform& pfrm,
                                       App& app,
                                       BasicCharacter& character)
{
    // Ok, so here, our ai character has boarded the player's island. We want to
    // find an ideal position to move the character to, with consideration to
    // the layout of the player's island. We can only move into a connected
    // component, and we want to attack a room with a high weight.

    // TODO: we may want to decrease the weight of a room if one of the player's
    // own characters stands between us and the target room. An unprotected room
    // is of course better than a guarded one.

    if (character.has_movement_path()) {
        // we've already assigned a movement path to the character. We may want
        // to recalculate the path at some point. But the computations below are
        // a bit expensive, so, instead, why don't we just defer the computation
        // until a room in the player's island is actually destroyed?
        return;
    }

    DynamicMemory<bool[16][16]> matrix_ = allocate_dynamic<bool[16][16]>(pfrm);

    app.player_island().plot_walkable_zones(*matrix_);

    u8 matrix[16][16];
    for (int x = 0; x < 16; ++x) {
        for (int y = 0; y < 16; ++y) {
            if ((*matrix_)[x][y]) {
                matrix[x][y] = 1;
            } else {
                matrix[x][y] = 0;
            }
        }
    }

    auto current_pos = character.grid_position();

    flood_fill(pfrm,
               matrix,
               2,
               current_pos.x,
               current_pos.y);

    struct Destination {
        Vec2<u8> coord_;
        Float ai_weight_;
    };

    Buffer<Destination, 48> slots;

    for (u8 x = 0; x < 16; ++x) {
        for (u8 y = 0; y < 16; ++y) {
            if (matrix[x][y] == 2) {
                slots.push_back({{x, y}, 0.f});
            }
        }
    }

    if (slots.empty()) {
        // TODO: what to do in this case? How can we even end up in a situation
        // like this? If all of the player's rooms were destroyed, and one of
        // the player's rooms retain a unique pointer to the invading enemy
        // character, then it should be theoretically impossible for us to end
        // up in this state.
        return;
    }

    for (auto& slot : slots) {
        if (auto room = app.player_island().get_room(slot.coord_)) {
            slot.ai_weight_ = (*room->metaclass())->ai_base_weight();
        }
    }

    std::sort(slots.begin(), slots.end(), [&](const Destination& lhs,
                                              const Destination& rhs) {
        return lhs.ai_weight_ < rhs.ai_weight_;
    });

    if (slots.back().ai_weight_ == 0.f) {
        // Again, perhaps this is overly defensive coding. But we should never
        // end up in a situation where the weights of the rooms are all
        // uninitialized...
        return;
    }

    auto target = slots.back();

    if (auto path = find_path(pfrm,
                              &app.player_island(),
                              current_pos,
                              target.coord_)) {
        character.set_movement_path(std::move(*path));
    }
}



void EnemyAI::set_target(Platform& pfrm,
                         App& app,
                         const u8 matrix[16][16],
                         MissileSilo& silo)
{
    if (silo.parent()->get_drift()) {
        // Wait until we've stopped moving
        return;
    }

    Buffer<Room*, 32> visible_rooms;

    for (int x = 0; x < 16; ++x) {
        for (int y = 0; y < 16; ++y) {
            if (matrix[x][y]) {
                if (auto room = app.player_island().get_room({u8(x), u8(y)})) {
                    visible_rooms.push_back(room);
                }
                break;
            }
        }
    }

    Room* highest_weighted_room = nullptr;
    Float highest_weight = 3E-5;

    auto cannon_metac = load_metaclass("cannon");

    bool cannons_remaining = false;

    for (auto& room : silo.parent()->rooms()) {
        if (room->metaclass() == cannon_metac) {
            cannons_remaining = true;
            break;
        }
    }

    for (auto room : visible_rooms) {
        auto meta_c = room->metaclass();
        auto w = (*meta_c)->ai_base_weight();

        // Give the room some extra weight, if firing a missile into it would be
        // really destructive.
        if (w > 400 and room->health() <= Missile::deals_damage) {
            w += 300.f;
        }

        // We don't have any cannons left, but the other player does. Try to
        // take out some of those cannons with missiles.
        if (meta_c == cannon_metac and not cannons_remaining) {
            w += 200.f;
        }

        if (w > highest_weight) {
            highest_weighted_room = room;
            highest_weight = w;
        }
    }

    if (highest_weighted_room) {
        silo.set_target(highest_weighted_room->position());
    }
}



void EnemyAI::set_target(Platform& pfrm,
                         App& app,
                         const u8 matrix[16][16],
                         Cannon& cannon)
{
    // Ok, lets start by finding all of the line-of-sight targets:
    // (FIXME: actually draw the line-of-sight correctly...)
    Buffer<Room*, 32> visible_rooms;

    for (u8 y = 0; y < 16; ++y) {
        for (int x = 15; x > -1; --x) {
            if (matrix[x][y]) {
                if (auto room = app.player_island().get_room({u8(x), y})) {
                    visible_rooms.push_back(room);
                }
                break;
            }
        }
    }

    Room* highest_weighted_room = nullptr;
    Float highest_weight = 3E-5;

    for (auto room : visible_rooms) {
        auto meta_c = room->metaclass();
        auto w = (*meta_c)->ai_base_weight();

        if (w > highest_weight) {
            highest_weighted_room = room;
            highest_weight = w;
        }
    }

    // Potentially attack the second highest weighted visible room, just to keep
    // things interesting.
    if (visible_rooms.size() > 1 and rng::choice<3>(rng::utility_state) == 0) {
        highest_weighted_room = visible_rooms[1];
    }

    if (highest_weighted_room) {
        cannon.set_target(highest_weighted_room->position());
    }
}



void EnemyAI::on_room_damaged(Platform& pfrm, App& app, Room& room)
{
    if (app.opponent_island()) {

        // bool weapons_remaining = false;
        // for (auto& room : app.opponent_island()->rooms()) {
        //     if (dynamic_cast<Cannon*>(&*room)) {
        //         weapons_remaining = true;
        //         break;
        //     }
        // }

        // if (not weapons_remaining) {
        //     // Ok, all of our weapons have been destroyed. I.E. we cannot
        //     // possibly win in this state. Now let's see what we can do...

        //     // Maybe we can build a cannons?
        //     auto cannon_mt = load_metaclass("cannon");
        //     if ((*cannon_mt)->cost() < coins_) {
        //         // Ok, so we actually have enough coins lying around to buy a
        //         // cannon!
        //         // Do we have a spot to place it?
        //         bool matrix[16][16];
        //         app.opponent_island()->plot_construction_zones(matrix);
        //         std::optional<Vec2<u8>> target;

        //         [&] {
        //             for (u8 y = 0; y < 16; ++y) {
        //                 for (u8 x = 0; x < 16; ++x) {
        //                     if (matrix[x][y]) {
        //                         target = {x, y};
        //                         return;
        //                     }
        //                 }
        //             }
        //         }();

        //         if (target) {
        //             (*cannon_mt)->create(pfrm, &*app.opponent_island(), *target);
        //             coins_ -= (*cannon_mt)->cost();
        //         }

        //     }

        // }

        // Things that we might want to do here:
        // 1) Initiate repairs
        // 2) Salvage the room (if it's about to be destroyed anyway)
        // 3) Build weapons or fortifications
    }
}



} // namespace skyland
