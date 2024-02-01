/*******************************************************************************************
*
*   raylib gamejam template
*
*   Template originally created with raylib 4.5-dev, last time updated with raylib 5.0
*
*   Template licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2022-2023 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"
#include "raymath.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include "res.h"

#if defined(PLATFORM_WEB)
    #define CUSTOM_MODAL_DIALOGS            // Force custom modal dialogs usage
    #include <emscripten/emscripten.h>      // Emscripten library - LLVM to JavaScript compiler
#endif

#include <stdio.h>                          // Required for: printf()
#include <stdlib.h>                         // Required for: 
#include <string.h>                         // Required for: 

//----------------------------------------------------------------------------------
// Defines and Macros
//----------------------------------------------------------------------------------
// Simple log system to avoid printf() calls if required
// NOTE: Avoiding those calls, also avoids const strings memory usage
#define SUPPORT_LOG_INFO
#if defined(SUPPORT_LOG_INFO)
    #define LOG(...) printf(__VA_ARGS__)
#else
    #define LOG(...)
#endif

//----------------------------------------------------------------------------------
// Global Variables Definition
//----------------------------------------------------------------------------------
static const int screenWidth = 576;
static const int screenHeight = 384;

static RenderTexture2D target = { 0 };  // Render texture to render our game

//----------------------------------------------------------------------------------
// Module Functions Declaration
//----------------------------------------------------------------------------------
static void UpdateDrawFrame(void);      // Update and Draw one frame

Material cube_mat = { 0 };

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    SetTraceLogLevel(LOG_ALL);         // Disable raylib trace log messsages

    // Initialization
    //--------------------------------------------------------------------------------------
    InitWindow(screenWidth, screenHeight, "crayjam");
    InitAudioDevice();
    SetMasterVolume(.7);

    res_init();
    PlaySound(good_snd);
    
    Texture cube_tex = LoadTexture("res/grass_tile.png");
    cube_mat = LoadMaterialDefault();
    SetMaterialTexture(&cube_mat, MATERIAL_MAP_DIFFUSE, cube_tex);
    
    // Render texture to draw full screen, enables screen scaling
    // NOTE: If screen is scaled, mouse input should be scaled proportionally
    target = LoadRenderTexture(screenWidth / 3, screenHeight / 3);
    // SetTextureFilter(target.texture, TEXTURE_FILTER_BILINEAR);

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(UpdateDrawFrame, 60, 1);
#else
    SetTargetFPS(60);     // Set our game frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button
    {
        UpdateDrawFrame();
    }
#endif

    UnloadMusicStream(music);
    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadRenderTexture(target);

    CloseAudioDevice();
    
    // TODO: Unload all loaded resources at this point

    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

typedef enum {
    GS_TITLE,
    GS_EXPLAIN,
    GS_WELCOME,
    GS_DAY,
    GS_WIN,
    GS_LOSE,
} game_state_t;

typedef enum {
    FAIL_BORED,
    FAIL_FIRED,
} fail_t;

typedef struct {
    game_state_t state;
    float state_time;
    int level;
    fail_t failure_mode;
    float score;
    int boredom[5];
    int levels[5][3][6];
    float last_time_score;
    float last_bonus_score;
} game_t;

game_t game = { 0 };

int point_map[3][6] = {
    { 0,-3, 4,-3, 0, 1},
    { 6, 4,-5, 3,-3, 3},
    {-4, 0, 0, 0, 5,-7},
};

void reset_game(){

    game_t g = {
        .state = GS_TITLE,
        .state_time = 0,
        .level = 0,
        .score = 0,
        .last_time_score = 0,
        .last_bonus_score = 0,
        .failure_mode = FAIL_BORED,
        .boredom = {2, 4, 6, 8, 9},
        .levels = {
            {
                {0, 1, 1, 0, 0, 0},
                {0, 0, 1, 1, 0, 1},
                {1, 2, 2, 2, 1, 0},
            },
            {
                {0, 1, 1, 0, 0, 0},
                {1, 0, 1, 1, 1, 1},
                {1, 2, 2, 2, 0, 0},
            },
            {
                {0, 0, 0, 1, 0, 1},
                {1, 1, 1, 0, 1, 0},
                {0, 2, 2, 2, 1, 1},
            },
            {
                {0, 1, 1, 1, 0, 1},
                {1, 1, 1, 0, 0, 1},
                {0, 2, 2, 2, 1, 1},
            },
            {
                {0, 1, 0, 0, 0, 1},
                {1, 0, 0, 0, 1, 1},
                {0, 2, 2, 2, 1, 1},
            },
        },
    };

    game = g;

}

void update_gs_title(){

    char * title = "OFFICE WORK";
    int size = 20;

    BeginTextureMode(target);
        ClearBackground(BLACK);

        DrawText(title, screenWidth / 3 / 2 - MeasureText(title, size) / 2, screenHeight / 3 / 4 - size / 2, size, RAYWHITE);
        
    EndTextureMode();
    
    // Render to screen (main framebuffer)
    BeginDrawing();
        DrawTexturePro(target.texture, (Rectangle){ 0, 0, (float)target.texture.width, -(float)target.texture.height }, (Rectangle){ 0, 0, (float)screenWidth, (float)screenHeight }, (Vector2){ 0, 0 }, 0.0f, WHITE);

        GuiSetStyle(DEFAULT, TEXT_SIZE, 30);
        if(GuiButton((Rectangle){screenWidth / 2. - 128, screenHeight / 4. * 3 - 48, 256, 96}, "GO TO WORK")){
            reset_game();
            game.state = GS_WELCOME;
            game.level = 1;
            game.state_time = GetTime();
            PlayMusicStream(music);
        }

    EndDrawing();

}

void update_gs_welcome(){

    char * mon = "MONDAY";
    char * tue = "TUESDAY";
    char * wed = "WEDNESDAY";
    char * thr = "THURSDAY";
    char * fri = "FRIDAY";

    char * curr = NULL;
    switch (game.level){
        case 1:
            curr = mon;
            break;
        case 2:
            curr = tue;
            break;
        case 3:
            curr = wed;
            break;
        case 4:
            curr = thr;
            break;
        case 5:
            curr = fri;
            break;
        default:
            break;
    }

    int size = 20;

    BeginTextureMode(target);
        ClearBackground(BLACK);
        DrawText(curr, screenWidth / 3 / 2 - MeasureText(curr, size) / 2, screenHeight / 3 / 4 - size / 2, size, RAYWHITE);
        if(game.level != 1){
            char time[50] = { 0 };
            char bons[50] = { 0 };
            char scor[50] = { 0 };

            sprintf(time, "time: +%.2f", game.last_time_score);
            sprintf(bons, "bonus: +%.2f", game.last_bonus_score);
            sprintf(scor, "score:  %.2f", game.score);

            DrawText(time, screenWidth / 3 / 2 - MeasureText(curr, size) / 2, screenHeight / 3 / 2 - 10, size / 2, GREEN);
            if (game.last_bonus_score != 0)
                DrawText(bons, screenWidth / 3 / 2 - MeasureText(curr, size) / 2, screenHeight / 3 / 2, size / 2, GREEN);
            DrawText(scor, screenWidth / 3 / 2 - MeasureText(curr, size) / 2, screenHeight / 3 / 2 + 10, size / 2, YELLOW);
        }
    EndTextureMode();

    if (GetTime() >= game.state_time + 2){
        game.state = GS_DAY;
        game.state_time = GetTime();
    }
    
    BeginDrawing();
        DrawTexturePro(target.texture, (Rectangle){ 0, 0, (float)target.texture.width, -(float)target.texture.height }, (Rectangle){ 0, 0, (float)screenWidth, (float)screenHeight }, (Vector2){ 0, 0 }, 0.0f, WHITE);
    EndDrawing();

}

void update_gs_day(){

    BeginTextureMode(target);
        ClearBackground(BLACK);

        DrawTexture(desk_tex, 0, 0, WHITE);

        Vector2 mp = Vector2Divide(GetMousePosition(), (Vector2){3,3});

        bool work_left = false;

        for(int x = 0; x < 6; x++){
            for(int y = 0; y < 3; y++){
                if(game.levels[game.level - 1][y][x] == 1){

                    if(point_map[y][x] < 0)
                        work_left = true;

                    DrawTexture(texture_map[y][x], x * 32, (y + 1) * 32, WHITE);

                    if(CheckCollisionPointPoly(mp, 
                        (Vector2[4]){
                            {x * 32      , (y + 1) * 32 }, // top left
                            {x * 32      , (y + 2) * 32 }, // bottom left
                            {(x + 1) * 32, (y + 2) * 32 }, // bottom right
                            {(x + 1) * 32, (y + 1) * 32 }, // top right
                        }, 4))
                    {

                        Color hc = RED;

                        if(point_map[y][x] > 0)
                            hc = GREEN;

                        char pts[2] = "0";

                        pts[0] = '0' + abs(point_map[y][x]);
                        int tw = MeasureText(pts, 10);
                        Color b = BLACK;
                        b.a = 127;
                        DrawRectangle(x * 32 + 11, (y + 1) * 32 + 11, 10, 11, b);
                        DrawText(pts, x * 32 + 11 + tw / 2, (y + 1) * 32 + 12, 10, hc);

                        if(IsMouseButtonReleased(MOUSE_LEFT_BUTTON)){
                            // maybe time or something
                            game.levels[game.level - 1][y][x] = 0;
                            game.boredom[game.level - 1] -= point_map[y][x];
                            game.boredom[game.level - 1] = Clamp(game.boredom[game.level - 1], 0, 10);
                            if(game.boredom[game.level - 1] >= 10){
                                game.state = GS_LOSE;
                                SeekMusicStream(music, 0);
                                StopMusicStream(music);
                            }

                            Sound *ps = &good_snd;
                            if(point_map[y][x] < 0)
                                ps = &bad__snd;

                            PlaySound(*ps);
                        }
                    }
                }
            }
        }

        Color dark_red = RED;
        dark_red.r = 255;
        dark_red.g /= 4;
        dark_red.b /= 4;

        DrawText("BOREDOM", 32 * 2 + 8, 4, 10, WHITE);
        DrawRectangle(32 * 2 + 4, 16, 64 - 8, 12, WHITE);
        DrawRectangleGradientH(32 * 2 + 6, 18, 64 - 8 - 4, 12 - 4, ORANGE, dark_red);
        DrawRectangle(70 + (52 * game.boredom[game.level - 1] / 10.), 18, 52 - (52 * game.boredom[game.level - 1] / 10.) + 1, 12 - 4, BLACK);
        // shitty float hacks
        DrawRectangle(122, 16, 2, 12, WHITE);
    EndTextureMode();

    float clock = (16 - (GetTime() - game.state_time)) / 2;
    if (clock <= 0){
        game.state = GS_LOSE;
        game.failure_mode = FAIL_FIRED;
        game.state_time = GetTime();
    }

    float clock_formatted = (int)clock;
    clock_formatted += fmodf(clock, 1) * .6;
    char time[10] = { 0 };
    sprintf(time, "%.2f", clock_formatted);
    time[1] = ':';

    // work done, next level
    if(!work_left){
        game.state = GS_WELCOME;
        game.last_time_score = game.level * (16. - (GetTime() - game.state_time)) / 2.;
        game.last_bonus_score = 0;
        // bonus points for unused distractions
        for(int x = 0; x < 6; x++){
            for(int y = 0; y < 3; y++){
                if(game.levels[game.level - 1][y][x] == 1 && point_map[y][x] > 0)
                    game.last_bonus_score += point_map[y][x] * game.level;
            }
        }
        game.level++;
        if (game.level == 6){
            game.state = GS_WIN;
            SeekMusicStream(music, 0);
            StopMusicStream(music);
        }
        game.score += game.last_time_score + game.last_bonus_score;
        game.state_time = GetTime();
    }
    
    // Render to screen (main framebuffer)
    BeginDrawing();
        DrawTexturePro(target.texture, (Rectangle){ 0, 0, (float)target.texture.width, -(float)target.texture.height }, (Rectangle){ 0, 0, (float)screenWidth, (float)screenHeight }, (Vector2){ 0, 0 }, 0.0f, WHITE);
        DrawText(time, 32 * 3 + 20, 22, 30, RED);
    EndDrawing();
}

void update_gs_lose(){

    char * title = "FAILURE";
    int size = 20;

    char * bored = "GOT TOO BORED";
    char * fired = "YOU GOT FIRED";
    char * reason = bored;
    if(game.failure_mode == FAIL_FIRED)
        reason = fired;

    char score[100] = {0};
    sprintf(score, "SCORE: %.2f", game.score);

    BeginTextureMode(target);
        ClearBackground(BLACK);
        DrawTextPro(GetFontDefault(), title, (Vector2){screenWidth / 3. / 2. + 10 - MeasureText(title, size) / 2., 10 }, (Vector2){0}, 15, size, 1, RED);
        DrawText(reason, screenWidth / 3 / 2 - MeasureText(reason, size / 2) / 2, screenHeight / 3 / 2 - size / 2, size / 2, RAYWHITE);
        DrawText(score, screenWidth / 3 / 2 - MeasureText(score, size / 2) / 2, screenHeight / 3 / 2, size / 2, YELLOW);
    EndTextureMode();
    
    // Render to screen (main framebuffer)
    BeginDrawing();
        DrawTexturePro(target.texture, (Rectangle){ 0, 0, (float)target.texture.width, -(float)target.texture.height }, (Rectangle){ 0, 0, (float)screenWidth, (float)screenHeight }, (Vector2){ 0, 0 }, 0.0f, WHITE);

        GuiSetStyle(DEFAULT, TEXT_SIZE, 30);
        if(GuiButton((Rectangle){screenWidth / 2. - 64, screenHeight / 4. * 3 - 48, 128, 96}, "MENU")){
            reset_game();
            game.state = GS_TITLE;
            game.level = 1;
            game.state_time = GetTime();
        }

    EndDrawing();

}

void update_gs_win(){

    char * title = "VICTORY";
    int size = 20;

    char * wknd = "MADE IT TO THE WEEKEND";

    char score[100] = {0};
    sprintf(score, "SCORE: %.2f", game.score);

    BeginTextureMode(target);
        ClearBackground(BLACK);
        DrawTextPro(GetFontDefault(), title, (Vector2){screenWidth / 3. / 2. + 10 - MeasureText(title, size) / 2., 10 }, (Vector2){0}, 15, size, 1, GREEN);
        DrawText(wknd, screenWidth / 3 / 2 - MeasureText(wknd, size / 2) / 2, screenHeight / 3 / 2 - size / 2, size / 2, RAYWHITE);
        DrawText(score, screenWidth / 3 / 2 - MeasureText(score, size / 2) / 2, screenHeight / 3 / 2, size / 2, YELLOW);
    EndTextureMode();
    
    // Render to screen (main framebuffer)
    BeginDrawing();
        DrawTexturePro(target.texture, (Rectangle){ 0, 0, (float)target.texture.width, -(float)target.texture.height }, (Rectangle){ 0, 0, (float)screenWidth, (float)screenHeight }, (Vector2){ 0, 0 }, 0.0f, WHITE);

        GuiSetStyle(DEFAULT, TEXT_SIZE, 30);
        if(GuiButton((Rectangle){screenWidth / 2. - 64, screenHeight / 4. * 3 - 48, 128, 96}, "MENU")){
            reset_game();
            game.state = GS_TITLE;
            game.level = 1;
            game.state_time = GetTime();
        }

    EndDrawing();

}

//--------------------------------------------------------------------------------------------
// Module functions definition
//--------------------------------------------------------------------------------------------
// Update and draw frame
void UpdateDrawFrame(void)
{
    // Update
    //----------------------------------------------------------------------------------
    // TODO: Update variables / Implement example logic at this point
    //----------------------------------------------------------------------------------

    switch(game.state){
        case GS_TITLE:
            update_gs_title();
            break;
        // case GS_EXPLAIN:
        //     update_gs_title();
        //     break;
        case GS_WELCOME:
            update_gs_welcome();
            UpdateMusicStream(music);
            break;
        case GS_DAY:
            update_gs_day();
            UpdateMusicStream(music);
            break;
        case GS_LOSE:
            update_gs_lose();
            break;
        case GS_WIN:
            update_gs_win();
            break;
        default:
            break;
    }

}
