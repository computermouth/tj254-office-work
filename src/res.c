
#include "res.h"
#include <raylib.h>

Texture desk_tex = { 0 };
Texture grn_tile_tex = { 0 };
Texture red_tile_tex = { 0 };

Texture spr_youtube   = { 0 };
Texture spr_meeting   = { 0 };
Texture spr_stamp     = { 0 };
Texture spr_documents = { 0 };
Texture spr_stapler   = { 0 };
Texture spr_coffee    = { 0 };
Texture spr_gameboy   = { 0 };
Texture spr_ball      = { 0 };
Texture spr_phone     = { 0 };
Texture spr_picture   = { 0 };
Texture spr_mail      = { 0 };
Texture spr_meme      = { 0 };
Texture spr_letters   = { 0 };

Texture texture_map[3][6] = { 0 };

Sound good_snd = { 0 };
Sound bad__snd = { 0 };
Music music = { 0 };

// int point_map[3][6] = {
//     { 0,-2, 2,-2, 0, 1},
//     { 6, 3,-4, 3,-2,-3},
//     { 4, 0, 0, 0, 2,-7},
// };

void res_init(){
    desk_tex = LoadTexture("res/desk.png");
    grn_tile_tex = LoadTexture("res/green_tile.png");
    red_tile_tex = LoadTexture("res/red_tile.png");

    spr_youtube   = LoadTexture("res/spr_yutub.png");         //
    spr_meeting   = LoadTexture("res/spr_meeting.png");       //
    spr_stamp     = LoadTexture("res/spr_stamp.png");         //
    spr_documents = LoadTexture("res/spr_documents.png");     //
    spr_stapler   = LoadTexture("res/spr_stapler.png");       //
    spr_coffee    = LoadTexture("res/spr_coffee.png");        //
    spr_gameboy   = LoadTexture("res/spr_gemboy.png");        //
    spr_ball      = LoadTexture("res/spr_ball.png");          //
    spr_phone     = LoadTexture("res/spr_gamesonphone.png");  //
    spr_picture   = LoadTexture("res/spr_picture.png");       //
    spr_mail      = LoadTexture("res/spr_mail.png");          //
    spr_meme      = LoadTexture("res/spr_meme.png");          //
    spr_letters   = LoadTexture("res/spr_letters.png");       //

    texture_map[0][0] = (Texture){0};
    texture_map[0][1] = spr_mail;
    texture_map[0][2] = spr_meme;
    texture_map[0][3] = spr_stapler;
    texture_map[0][4] = (Texture){0};
    texture_map[0][5] = spr_picture;
    texture_map[1][0] = spr_gameboy;
    texture_map[1][1] = spr_youtube;
    texture_map[1][2] = spr_meeting;
    texture_map[1][3] = spr_coffee;
    texture_map[1][4] = spr_letters;
    texture_map[1][5] = spr_ball;
    texture_map[2][0] = spr_stamp;
    texture_map[2][1] = (Texture){0};
    texture_map[2][2] = (Texture){0};
    texture_map[2][3] = (Texture){0};
    texture_map[2][4] = spr_phone;
    texture_map[2][5] = spr_documents;

    good_snd = LoadSound("res/good.wav");
    bad__snd = LoadSound("res/bad.wav");
    music = LoadMusicStream("res/drozerix_-_computer_fuck!.xm");
    SetMusicVolume(music, .3);
    music.looping = true;

}
