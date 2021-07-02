// clang-format off

// This file was generated by the project's image build pipeline in
// CMakeLists.txt. I have no idea why cmake outputs semicolons between the
// entries in the IMAGE_INCLUDES list variable... maybe some special formatting
// character that I don't know about... or maybe cmake uses ; internally as a
// delimiter... but for now, the cmake script outputs the semicolons on a new
// line, commented out, to suppress the resulting errors.
//
// If you try to manually edit source/platform/gba/images.cpp, the file may be
// overwritten if you have the GBA_AUTOBUILD_IMG option in the cmake environment
// turned on.



#include "data/spritesheet.h"
//;
#include "data/spritesheet_title_screen.h"
//;
#include "data/tilesheet.h"
//;
#include "data/tilesheet_enemy_0.h"
//;
#include "data/tilesheet_enemy_0_interior.h"
//;
#include "data/tilesheet_interior.h"
//;
#include "data/tilesheet_top.h"
//;
#include "data/skyland_title_1_flattened.h"
//;
#include "data/skyland_title_0_flattened.h"
//;
#include "data/zone_image_0_flattened.h"
//;
#include "data/zone_image_1_flattened.h"
//;
#include "data/overlay.h"
//;
#include "data/overlay_dialog.h"
//;
#include "data/charset.h"
//;
#include "data/overlay_world_map.h"
//;
#include "data/overlay_island_destroyed.h"
//;
#include "data/overlay_challenges.h"
//;
#include "data/hint_infirmary.h"
//;
#include "data/hint_goblin.h"
//;
#include "data/hint_plunder.h"
//

struct TextureData {
    const char* name_;
    const unsigned int* tile_data_;
    const unsigned short* palette_data_;
    u32 tile_data_length_;
    u32 palette_data_length_;
};


#define STR(X) #X
#define TEXTURE_INFO(NAME)                                                     \
    {                                                                          \
        STR(NAME), NAME##Tiles, NAME##Pal, NAME##TilesLen, NAME##PalLen        \
    }


static const TextureData sprite_textures[] = {

    TEXTURE_INFO(spritesheet),
//;
    TEXTURE_INFO(spritesheet_title_screen),
//
};


static const TextureData tile_textures[] = {

    TEXTURE_INFO(tilesheet),
//;
    TEXTURE_INFO(tilesheet_enemy_0),
//;
    TEXTURE_INFO(tilesheet_enemy_0_interior),
//;
    TEXTURE_INFO(tilesheet_interior),
//;
    TEXTURE_INFO(tilesheet_top),
//;
    TEXTURE_INFO(skyland_title_1_flattened),
//;
    TEXTURE_INFO(skyland_title_0_flattened),
//;
    TEXTURE_INFO(zone_image_0_flattened),
//;
    TEXTURE_INFO(zone_image_1_flattened),
//
};


static const TextureData overlay_textures[] = {

    TEXTURE_INFO(overlay),
//;
    TEXTURE_INFO(overlay_dialog),
//;
    TEXTURE_INFO(charset),
//;
    TEXTURE_INFO(overlay_world_map),
//;
    TEXTURE_INFO(overlay_island_destroyed),
//;
    TEXTURE_INFO(overlay_challenges),
//;
    TEXTURE_INFO(hint_infirmary),
//;
    TEXTURE_INFO(hint_goblin),
//;
    TEXTURE_INFO(hint_plunder),
//
};

// clang-format on
