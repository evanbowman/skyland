#pragma once


#include "skyland/scene/module.hpp"



namespace skyland {



class DlcInjectorModule : public Module<DlcInjectorModule> {
public:
    static const char* module_name()
    {
        return "DLC Loader";
    }


    static u16 icon()
    {
        return 968;
    }


    ScenePtr<Scene> update(Platform&, App&, Microseconds delta) override;


    static Factory factory_;
};



} // namespace skyland
