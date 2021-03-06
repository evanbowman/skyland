#include "factoryResetModule.hpp"
#include "platform/ram_filesystem.hpp"
#include "skyland/scene/titleScreenScene.hpp"
#include "skyland/skyland.hpp"
#include "skyland/save.hpp"



namespace skyland {



ScenePtr<Scene>
FactoryResetModule::update(Platform& pfrm, App& app, Microseconds delta)
{
    if (not text_) {
        pfrm.screen().fade(0.9f);
        pfrm.screen().fade(1.f);
        text_.emplace(pfrm);
        text_->assign("Reset the game to factory settings? "
                      "Press B to cancel. Press A five times "
                      "to continue.",
                      {1, 1},
                      {28, 8});
    }

    if (app.player().key_down(pfrm, Key::action_2)) {
        text_.reset();
        return scene_pool::alloc<TitleScreenScene>();
    }

    if (app.player().key_down(pfrm, Key::action_1)) {
        ++key_count_;
        if (key_count_ == 5) {
            save::GlobalSaveData save;
            save.magic_.set(0xBADF00D);
            pfrm.write_save_data(&save, sizeof save, 0);
            ram_filesystem::destroy(pfrm);
            pfrm.restart();
        }
    }

    return null_scene();
}



FactoryResetModule::Factory FactoryResetModule::factory_;



} // namespace skyland
