#pragma once


struct Tile {
    enum {
        terrain_middle = 12,
        terrain_left,
        terrain_right,
        wall_window_1,
        wall_window_2,
        wall_plain_1,
        wall_plain_2,
        roof_plain,
        roof_chimney,
        cannon_1,
        armored_wall_1,
        armored_wall_2,
        roof_plain_intersect_armored_wall,
        wall_window_middle_1,
        wall_window_middle_2,
        grass,
        reserved_1,
        hull,
        wall_plain_middle,
        reserved_3,
        reserved_4,
        reserved_5,
        reserved_6,
        missile_silo_1,
        missile_silo_2,
        forcefield,
        reserved7,
        reserved8,
        reserved9,
        reserved10,
        tin_chimney,
        plundered_room,
        reserved_11,
        reserved_12,
        radar_1,
        radar_2,
        reserved_13,
        reserved_14,
        reserved_15,
        reserved_16,
        particle_gun,
        reserved_17,
        flag_start = 105,
        flag_end = 108,
        roof_flag = 109,
        flag_mount
    };
};


struct InteriorTile {
    enum {
        core_1 = 15,
        core_2,
        core_3,
        core_4,
        roof_1,
        roof_2,
        cannon_1,
        ladder_base,
        ladder_mid,
        roof_3,
        ladder_mid_2,
        ladder_top,
        grass,
        empty,
        hull,
        workshop_1,
        workshop_2,
        workshop_3,
        workshop_4,
        path_marker,
        missile_silo_1,
        missile_silo_2,
        forcefield,
        infirmary_1,
        infirmary_3,
        infirmary_2,
        infirmary_4,
        tin_chimney,
        plundered_room,
        transporter_1,
        transporter_2,
        radar_1,
        radar_2,
        bulkhead_closed_1,
        bulkhead_closed_2,
        bulkhead_open_1,
        bulkhead_open_2,
        particle_gun,
        transporter_recharge,
    };
};
