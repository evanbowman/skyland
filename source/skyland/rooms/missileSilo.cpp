#include "missileSilo.hpp"
#include "platform/platform.hpp"
#include "skyland/alloc_entity.hpp"
#include "skyland/entity/projectile/missile.hpp"
#include "skyland/scene/weaponSetTargetScene.hpp"
#include "skyland/scene_pool.hpp"
#include "skyland/skyland.hpp"
#include "skyland/tile.hpp"



namespace skyland {



MissileSilo::MissileSilo(Island* parent, const Vec2<u8>& position)
    : Room(parent, name(), size(), position)
{
}



void MissileSilo::update(Platform& pfrm, App& app, Microseconds delta)
{
    Room::update(pfrm, app, delta);

    if (load_ > 0) {
        load_ -= delta;
    } else {
        if (target_) {
            auto island = other_island(app);

            if (parent()->power_supply() < parent()->power_drain()) {
                return;
            }

            if (island) {
                if (target_) {
                    if (auto room = island->get_room(*target_)) {
                        auto start = center();
                        start.y -= 24;

                        app.camera().shake(6);
                        load_ = load_time;
                        auto m = app.alloc_entity<Missile>(
                            pfrm, start, room->center(), parent());

                        if (m) {
                            parent()->projectiles().push(std::move(m));
                        }
                    }
                }
            }
        }
    }
}



void MissileSilo::render_interior(u8 buffer[16][16])
{
    buffer[position().x][position().y] = Tile::missile_silo_1;
    buffer[position().x][position().y + 1] = Tile::missile_silo_2;
}



void MissileSilo::render_exterior(u8 buffer[16][16])
{
    buffer[position().x][position().y] = Tile::missile_silo_1;
    buffer[position().x][position().y + 1] = Tile::missile_silo_2;
}


ScenePtr<Scene> MissileSilo::select(Platform& pfrm, App& app)
{
    const auto& mt_prep_seconds =
        std::get<SkylandGlobalData>(globals()).multiplayer_prep_seconds_;

    if (mt_prep_seconds) {
        return null_scene();
    }

    return scene_pool::alloc<WeaponSetTargetScene>(position());
}


} // namespace skyland
