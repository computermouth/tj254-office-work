
#ifndef JAM_RES_H
#define JAM_RES_H

#include <raylib.h>

extern Texture desk_tex;
extern Texture grn_tile_tex;
extern Texture red_tile_tex;

extern Texture texture_map[3][6];

extern Texture spr_youtube  ;
extern Texture spr_meeting  ;
extern Texture spr_stamp    ;
extern Texture spr_documents;
extern Texture spr_stapler  ;
extern Texture spr_coffee   ;
extern Texture spr_gameboy  ;
extern Texture spr_ball     ;
extern Texture spr_phone    ;
extern Texture spr_picture  ;
extern Texture spr_mail     ;
extern Texture spr_meme     ;
extern Texture spr_letters  ;

extern Sound good_snd;
extern Sound bad__snd;
extern Music music;

void res_init();

#endif
