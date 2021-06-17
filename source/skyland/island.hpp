#pragma once

#include "bulkAllocator.hpp"
#include "entity.hpp"
#include "memory/buffer.hpp"
#include "room.hpp"
#include "roomPool.hpp"
#include "player.hpp"
#include "entity/character/basicCharacter.hpp"



namespace skyland {


class Island {
public:
    Island(Platform& pfrm, Layer layer, u8 width, Player& player);


    using Rooms = Buffer<RoomPtr<Room>, 20>;


    bool add_room(Platform& pfrm, RoomPtr<Room> insert)
    {
        auto result = rooms_.push_back(std::move(insert));
        repaint(pfrm);
        return result;
    }


    template <typename T>
    bool add_room(Platform& pfrm, const Vec2<u8>& position)
    {
        if (auto room = room_pool::alloc<T>(this, position)) {
            if (rooms_.push_back({room.release(), room_pool::deleter})) {
                repaint(pfrm);
                return true;
            }
        }
        pfrm.fatal("room pool exhausted");
        return false;
    }


    bool add_character(EntityRef<BasicCharacter> character);



    Rooms& rooms();


    void update(Platform&, App&, Microseconds delta);


    void display(Platform&);


    const Vec2<Float>& get_position() const;


    void set_position(const Vec2<Float>& position);


    Room* get_room(const Vec2<u8>& coord);


    void destroy_room(Platform& pfrm, const Vec2<u8>& coord);


    u8 get_ambient_movement()
    {
        return ambient_movement_;
    }


    Layer layer() const
    {
        return layer_;
    }


    void render_interior(Platform& pfrm);


    void render_exterior(Platform& pfrm);


    void plot_rooms(u8 matrix[16][16]) const;


    void plot_construction_zones(bool matrix[16][16]) const;


    void plot_walkable_zones(bool matrix[16][16]) const;


    void repaint(Platform& pfrm);


    bool interior_visible() const
    {
        return interior_visible_;
    }


    Vec2<Float> origin() const;


    using Terrain = Buffer<u8, 10>;


    Terrain& terrain()
    {
        return terrain_;
    }


    void render_terrain(Platform& pfrm);


    void set_float_timer(Microseconds value);


    void show_flag(bool show)
    {
        show_flag_ = show;
    }


    void set_drift(Float drift);


    Float get_drift() const
    {
        return drift_;
    }


    EntityList<Entity>& projectiles()
    {
        return projectiles_;
    }


    void test_collision(Platform&, App&, Entity& entity);


    Player& owner()
    {
        return player_;
    }


    bool is_destroyed()
    {
        return destroyed_;
    }


    std::optional<Vec2<u8>> chimney_loc() const
    {
        return chimney_loc_;
    }



private:
    Rooms rooms_;
    const Layer layer_;
    Buffer<u8, 10> terrain_;
    Vec2<Float> position_;
    u8 ambient_movement_;
    Microseconds timer_;
    Float drift_ = 0;

    bool interior_visible_;
    bool show_flag_ = false;

    EntityList<BasicCharacter> characters_;
    EntityList<Entity> projectiles_;

    Player& player_;

    bool destroyed_ = false;

    std::optional<Vec2<u8>> chimney_loc_;
    Microseconds chimney_spawn_timer_ = 0;
};



} // namespace skyland
