
#include <stdio.h>

#include "entity_banner.h"
#include "entity.h"
#include "game.h"
#include "raylib.h"
#include "vector.h"

void entity_banner_draw_2d(Entity * b);
void entity_banner_draw_3d(Entity * b);

float get_bottom_tts_banner(){
    float bb = 0;

    vector * oev = game_get_other_entities();
    size_t len = vector_size(oev);
    Entity * oe = vector_begin(oev);
    for(size_t i = 0; i < len; i++){
        if (
            !oe[i].dead &&
            oe[i].type == ENTITY_TYPE_BANNER &&
            oe[i].banner_storage.pos.y > bb && 
            (
                oe[i].banner_storage.type == BANNER_TTS_LVL_ONE ||
                oe[i].banner_storage.type == BANNER_TTS_LVL_TWO ||
                oe[i].banner_storage.type == BANNER_TTS_LVL_THREE ||
                oe[i].banner_storage.type == BANNER_TTS_LVL_FOUR ||
                oe[i].banner_storage.type == BANNER_TTS_LVL_FIVE ||
                oe[i].banner_storage.type == BANNER_TTS_LVL_INFINITE ||
                oe[i].banner_storage.type == BANNER_TTS_LVL_ENGAGE ||
                oe[i].banner_storage.type == BANNER_TTS_WPN_PU ||
                oe[i].banner_storage.type == BANNER_TTS_SHD_PU ||
                oe[i].banner_storage.type == BANNER_TTS_MULTI ||
                oe[i].banner_storage.type == BANNER_TTS_FAST_CLEAR
            )
        ){
            bb = oe[i].banner_storage.pos.y + 40;
        }
    }

    if (bb > 100){
        return bb;
    }

    return 100;
}

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

    // set bottom banner
    if (
        bt == BANNER_TTS_LVL_ONE ||
        bt == BANNER_TTS_LVL_TWO ||
        bt == BANNER_TTS_LVL_THREE ||
        bt == BANNER_TTS_LVL_FOUR ||
        bt == BANNER_TTS_LVL_FIVE ||
        bt == BANNER_TTS_LVL_INFINITE ||
        bt == BANNER_TTS_LVL_ENGAGE ||
        bt == BANNER_TTS_WPN_PU ||
        bt == BANNER_TTS_SHD_PU ||
        bt == BANNER_TTS_MULTI ||
        bt == BANNER_TTS_FAST_CLEAR
    ){
        bs.pos.y = get_bottom_tts_banner();
    }

    b.banner_storage = bs;

    return b;
}

void entity_xp_banner_update(Entity * banner){
    banner->banner_storage.pos.y -= 50 * game_get_delta();

    // 2s timeout 
    if ( game_get_time() - banner->banner_storage.origin_time > 2 ){
        banner->dead = true;
    }
}

void entity_tts_banner_update(Entity * banner){
    banner->banner_storage.pos.y -= 50 * game_get_delta();

    // off screen
    if (banner->banner_storage.pos.y < -40){
        banner->dead = true;
    }
}

void entity_banner_update(Entity * banner){

    // xp
    if (
        banner->banner_storage.type == BANNER_XPUP_RED ||
        banner->banner_storage.type == BANNER_XPUP_GRN ||
        banner->banner_storage.type == BANNER_XPUP_BLU
    ){
        entity_xp_banner_update(banner);
        return;
    }

    // xp
    if (
        banner->banner_storage.type == BANNER_TTS_LVL_ONE ||
        banner->banner_storage.type == BANNER_TTS_LVL_TWO ||
        banner->banner_storage.type == BANNER_TTS_LVL_THREE ||
        banner->banner_storage.type == BANNER_TTS_LVL_FOUR ||
        banner->banner_storage.type == BANNER_TTS_LVL_FIVE ||
        banner->banner_storage.type == BANNER_TTS_LVL_INFINITE ||
        banner->banner_storage.type == BANNER_TTS_LVL_ENGAGE ||
        banner->banner_storage.type == BANNER_TTS_WPN_PU ||
        banner->banner_storage.type == BANNER_TTS_SHD_PU ||
        banner->banner_storage.type == BANNER_TTS_MULTI ||
        banner->banner_storage.type == BANNER_TTS_FAST_CLEAR
    ){
        entity_tts_banner_update(banner);
        return;
    }

}

Color banner_colors[__BANNER_SIZE_END][2] = {
    [BANNER_XPUP_RED] = {RED  , {76, 13, 18, 255}},
    [BANNER_XPUP_GRN] = {GREEN, { 0, 76, 16, 255}},
    [BANNER_XPUP_BLU] = {BLUE , { 0, 38, 76, 255}},
};

void entity_xp_banner_draw(Entity *b){
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

void entity_tts_banner_draw(Entity *b){

    char * tts[] = {
        "LEVEL ONE",
        "LEVEL TWO",
        "LEVEL THREE",
        "LEVEL FOUR",
        "LEVEL FIVE",
        "INFINITE MODE",
        "ENGAGE",
        "+WEAPON PWR",
        "+SHIELD PWR",
        "+MULTI",
        "FAST CLEAR BONUS"
    };

    char * val = tts[b->banner_storage.type - BANNER_TTS_LVL_ONE];
    int font_size = 32;
    float tw = MeasureText(val, font_size);

    Color bc_back = BLACK;
    Color bc_fore = RAYWHITE;

    bc_back.a = 255;
    bc_fore.a = 255;

    if (b->banner_storage.pos.y <=100){
        bc_back.a = b->banner_storage.pos.y + 50. / 150 * 255;
        bc_fore.a = b->banner_storage.pos.y + 50. / 150 * 255;
    }

    DrawText(val, b->banner_storage.pos.x - tw + 2, b->banner_storage.pos.y + 2, font_size, bc_back);
    DrawText(val, b->banner_storage.pos.x - tw    , b->banner_storage.pos.y    , font_size, bc_fore);
}

void entity_banner_draw_2d(Entity * banner){

    // xp
    if (
        banner->banner_storage.type == BANNER_XPUP_RED ||
        banner->banner_storage.type == BANNER_XPUP_GRN ||
        banner->banner_storage.type == BANNER_XPUP_BLU
    ){
        entity_xp_banner_draw(banner);
        return;
    }

    // xp
    if (
        banner->banner_storage.type == BANNER_TTS_LVL_ONE ||
        banner->banner_storage.type == BANNER_TTS_LVL_TWO ||
        banner->banner_storage.type == BANNER_TTS_LVL_THREE ||
        banner->banner_storage.type == BANNER_TTS_LVL_FOUR ||
        banner->banner_storage.type == BANNER_TTS_LVL_FIVE ||
        banner->banner_storage.type == BANNER_TTS_LVL_INFINITE ||
        banner->banner_storage.type == BANNER_TTS_LVL_ENGAGE ||
        banner->banner_storage.type == BANNER_TTS_WPN_PU ||
        banner->banner_storage.type == BANNER_TTS_SHD_PU ||
        banner->banner_storage.type == BANNER_TTS_MULTI ||
        banner->banner_storage.type == BANNER_TTS_FAST_CLEAR
    ){
        entity_tts_banner_draw(banner);
        return;
    }
}

void entity_banner_draw_3d(Entity * b){
    (void)b;
}
