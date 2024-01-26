
#include "res.h"
#include <raylib.h>

Texture desk_tex = { 0 };
Texture grn_tile_tex = { 0 };
Texture red_tile_tex = { 0 };


Texture texture_map[3][6] = { 0 };

void res_init(){
    desk_tex = LoadTexture("res/desk.png");
    grn_tile_tex = LoadTexture("res/green_tile.png");
    red_tile_tex = LoadTexture("res/red_tile.png");

    texture_map[0][0] = red_tile_tex;
    texture_map[0][1] = grn_tile_tex;
    texture_map[0][2] = red_tile_tex;
    texture_map[0][3] = grn_tile_tex;
    texture_map[0][4] = grn_tile_tex;
    texture_map[0][5] = red_tile_tex;
    texture_map[1][0] = grn_tile_tex;
    texture_map[1][1] = red_tile_tex;
    texture_map[1][2] = grn_tile_tex;
    texture_map[1][3] = (Texture){0};
    texture_map[1][4] = red_tile_tex;
    texture_map[1][5] = grn_tile_tex;
    texture_map[2][0] = red_tile_tex;
    texture_map[2][1] = (Texture){0};
    texture_map[2][2] = (Texture){0};
    texture_map[2][3] = red_tile_tex;
    texture_map[2][4] = grn_tile_tex;
    texture_map[2][5] = red_tile_tex;

}