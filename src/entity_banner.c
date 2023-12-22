
#include <stdio.h>

#include "entity_banner.h"
#include "entity.h"
#include "game.h"
#include "raylib.h"

void entity_banner_draw_2d();
void entity_banner_draw_3d();

Entity entity_banner_spawn(BannerType bt, Vector2 screen_pos){

    Entity b = {
        .draw_2d_fn = entity_banner_draw_2d,
        .draw_3d_fn = entity_banner_draw_3d,
        .type = ENTITY_TYPE_BANNER,
        .dead = false,
    };

    BannerStorage bs = {
        .origin_time = game_get_time(),
        .pos = screen_pos,
        .type = bt,
    };

    b.banner_storage = bs;

    return b;
}

void entity_banner_update(Entity * banner){
    banner->banner_storage.pos.y -= 50 * game_get_delta();

    if (
        banner->banner_storage.type <= BANNER_XPUP_BLU &&
        game_get_time() - banner->banner_storage.origin_time > 2 // 2s timeout
    ){
        banner->dead = true;
    }
}

Color banner_colors[__BANNER_SIZE_END][2] = {
    [BANNER_XPUP_RED] = {RED  , {76, 13, 18, 255}},
    [BANNER_XPUP_GRN] = {GREEN, { 0, 76, 16, 255}},
    [BANNER_XPUP_BLU] = {BLUE , { 0, 38, 76, 255}},
};

void entity_banner_draw_2d(Entity * b){
    char * text = "+xp";
    int font_size = 32;
    float tw = MeasureText(text, font_size);

    Color bc_back = banner_colors[b->banner_storage.type][1];
    Color bc_fore = banner_colors[b->banner_storage.type][0];

    bc_back.a = -(game_get_time() - 2 - b->banner_storage.origin_time) * 255 / 2;
    bc_fore.a = -(game_get_time() - 2 - b->banner_storage.origin_time) * 255 / 2;

    DrawText(text, b->banner_storage.pos.x - 2 - tw / 2, b->banner_storage.pos.y + 2, font_size, bc_back);
    DrawText(text, b->banner_storage.pos.x     - tw / 2, b->banner_storage.pos.y    , font_size, bc_fore);
}

void entity_banner_draw_3d(Entity * b){
    (void)b;
}