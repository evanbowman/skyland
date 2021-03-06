#include "flak.hpp"
#include "skyland/alloc_entity.hpp"
#include "skyland/entity/explosion/explosion.hpp"
#include "skyland/entity/misc/smokePuff.hpp"
#include "skyland/room.hpp"
#include "skyland/room_metatable.hpp"
#include "skyland/rooms/cannon.hpp"
#include "skyland/rooms/forcefield.hpp"

#include "localization.hpp"


namespace skyland {



Flak::Flak(const Vec2<Float>& position,
           const Vec2<Float>& target,
           Island* source,
           const Vec2<u8>& origin_tile)
    : Projectile({{10, 10}, {8, 8}}), source_(source), origin_tile_(origin_tile)
{
    sprite_.set_position(position);
    sprite_.set_size(Sprite::Size::w16_h32);
    sprite_.set_texture_index(60);

    sprite_.set_origin({9, 9});

    static const Float speed = 0.00015f;
    const auto dir = direction(position, target);
    step_vector_ = dir * speed;
}



void Flak::update(Platform&, App&, Microseconds delta)
{
    auto pos = sprite_.get_position();
    pos = pos + Float(delta) * step_vector_;
    sprite_.set_position(pos);

    timer_ += delta;

    if (timer_ > seconds(2)) {
        kill();
    }

    flicker_time_ += delta;
    if (flicker_time_ > milliseconds(150)) {
        flicker_time_ = 0;

        if (sprite_.get_mix().amount_) {
            sprite_.set_mix({});
        } else {
            sprite_.set_mix({ColorConstant::silver_white, 200});
        }
    }
}



void Flak::on_collision(Platform& pfrm, App& app, Room& room)
{
    if (destroyed_) {
        return;
    }

    if (source_ == room.parent()) {
        if (room.position().x == origin_tile_.x or
            room.position().x + 1 == origin_tile_.x) {
            // Because we do not want to include collisions with the originating
            // cannon, or with any blocks directly above or below the cannon.
            return;
        }
    }

    if (source_ == room.parent() and room.metaclass() == forcefield_mt) {
        return;
    }


    kill();
    app.camera().shake(8);
    big_explosion(pfrm, app, sprite_.get_position());

    auto flak_smoke = [](Platform& pfrm, App& app, const Vec2<Float>& pos) {
        auto e = app.alloc_entity<SmokePuff>(
            pfrm, rng::sample<48>(pos, rng::utility_state), 61);

        if (e) {
            app.effects().push(std::move(e));
        }
    };

    flak_smoke(pfrm, app, sprite_.get_position());
    flak_smoke(pfrm, app, sprite_.get_position());


    app.on_timeout(pfrm,
                   milliseconds(190),
                   [pos = sprite_.get_position(), flak_smoke](
                       Platform& pf, App& app) { flak_smoke(pf, app, pos); });



    // Ok, so now we want to find the nearest tile with which we collided.
    auto origin = room.origin();
    // Go from unconstrained coordinates to an index in the opponent's tile grid.
    int y_offset = (origin.y - sprite_.get_position().y) / 16 + 1;
    int grid_y_start = room.position().y + y_offset;

    int x_offset = (origin.x - sprite_.get_position().x) / 16;

    if (room.parent() == &app.player_island()) {
        ++x_offset;
    }

    int grid_x_start = room.position().x + x_offset;

    // grid_x_start = clamp(x_offset, 0, 15);
    // grid_y_start = clamp(y_offset, 0, 15);

    // Alright, now we have grid coordinates at which to apply the flak damage.

    auto apply_damage = [&](int x_off, int y_off, Health damage) {
        auto island = room.parent();
        const int x = grid_x_start + x_off;
        const int y = grid_y_start + y_off;
        if (x >= 0 and x < 16 and y >= 0 and y < 16) {
            if (auto room = island->get_room({u8(x), u8(y)})) {
                room->apply_damage(pfrm, app, damage);
            }
        }
    };

    // Apply damage in this pattern:
    //         *
    //       * * *
    //     * * * * *
    //       * * *
    //         *
    //
    // More damage at center of explosion.

    apply_damage(0, 0, r1_damage);

    apply_damage(1, 0, r2_damage);
    apply_damage(-1, 0, r2_damage);
    apply_damage(0, 1, r2_damage);
    apply_damage(0, -1, r2_damage);

    apply_damage(2, 0, r3_damage);
    apply_damage(1, -1, r3_damage);
    apply_damage(0, -2, r3_damage);
    apply_damage(-1, -1, r3_damage);
    apply_damage(-2, 0, r3_damage);
    apply_damage(-1, 1, r3_damage);
    apply_damage(0, 2, r3_damage);
    apply_damage(1, 1, r3_damage);

    destroyed_ = true;
}



} // namespace skyland
