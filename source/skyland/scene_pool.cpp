#include "scene_pool.hpp"
#include "scene/worldMapScene.hpp"


namespace skyland {


namespace scene_pool {


_Pool* pool_ = nullptr;


}


ScenePtr<Scene> null_scene()
{
    return {nullptr, scene_pool::deleter};
}


ScenePtr<Scene> initial_scene()
{
    return scene_pool::alloc<WorldMapScene>();
}


} // namespace skyland
