/*******************************************************************************************
*
*   raylib [audio] example - Music playing (streaming)
*
*   Example originally created with raylib 1.3, last time updated with raylib 4.0
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2015-2022 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"
#include <stdio.h>

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "LuckFY");
    SetWindowState(FLAG_WINDOW_ALWAYS_RUN);
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    InitAudioDevice();              // Initialize audio device

    Music music[3] = {LoadMusicStream("songs/Shawn Mendes - Stitches.mp3"),
                      LoadMusicStream("songs/Jao - Meninos e Meninas.mp3"),
                      LoadMusicStream("songs/Dove Cameron - Boyfriend.mp3")};

    Texture2D capaAlbum[3] = {LoadTexture("capaAlbum/Shawn Mendes - Stitches.png"),
                         LoadTexture("capaAlbum/Jao - Meninos e Meninas.png"),
                         LoadTexture("capaAlbum/Dove Cameron - Boyfriend.png")};

    char autorEmusica[3][41]= {"Shawn Mendes - Stitches", "Jao - Meninos e Meninas", "Dove Cameron - Boyfriend"};

    Rectangle recF[3] = {200, 350, 400, 5,
                         200,340,400, 20};


    float timePlayed = 0.0f;        // Time played normalized [0.0f..1.0f]
    bool pause = false;
         // Music playing paused
    float mouse;
    int musica=0;
    float volume = 0.01;

    for(musica = 0; musica <3 ; musica++)
        PlayMusicStream(music[musica]);

    SetTargetFPS(165);               // Set our game to run at 30 frames-per-second
    //--------------------------------------------------------------------------------------
    musica = 0;
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        DrawRectangle(0, 0, screenWidth, screenHeight, DARKGRAY);
        UpdateMusicStream(music[musica]);   // Update music buffer with new stream data
        SetMusicVolume(music[musica], volume);
        Rectangle rec = {200, 350, (int)(timePlayed*400.0f), 5};

        // Volume +
        if (IsKeyPressed(KEY_UP) && volume < 1)
        {
            volume+=0.05;
            printf("%f\n", volume);
        }

        // Volume -
        if (IsKeyPressed(KEY_DOWN) && volume > 0)
        {
            volume-=0.05;
            printf("%f\n", volume);
        }

        // Restart music playing (stop and play)
        if (IsKeyPressed(KEY_SPACE))
        {
            StopMusicStream(music[musica]);
            PlayMusicStream(music[musica]);
        }

        // Pause/Resume music playing
        if (IsKeyPressed(KEY_P))
        {
            pause = !pause;

            if (pause) PauseMusicStream(music[musica]);
            else ResumeMusicStream(music[musica]);
        }

        // Get normalized time played for current music stream
        timePlayed = GetMusicTimePlayed(music[musica])/GetMusicTimeLength(music[musica]);
        if (timePlayed > 1.0f)
            timePlayed = 1.0f;  // Make sure time played is no longer than music
        if (GetMusicTimePlayed(music[musica]) >= GetMusicTimeLength(music[musica]) - 0.01 && (musica == 0 || musica == 1))
            musica++;
        //----------------------------------------------------------------------------------

        //----------------------------------------------------------------------------------
        BeginDrawing();

            DrawTexture(capaAlbum[musica],300,100,WHITE);
            DrawText(TextFormat("%s", autorEmusica[musica]), 210, 320, 20, LIGHTGRAY);
            DrawRectangleRounded(recF[0], 15, 30, GRAY);
            DrawRectangleRounded(rec, 15, 30, GREEN);

            if (CheckCollisionPointRec(GetMousePosition(), recF[1]))
            {
                DrawCircle((int)(timePlayed*400.0f)+200,353,6,WHITE);
                if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
                {
                    mouse = GetMouseX();
                    if(GetMouseX() <= 200)
                        SeekMusicStream(music[musica],0);
                    else
                        SeekMusicStream(music[musica], GetMusicTimeLength(music[musica])*((mouse+200)*0.0025) - GetMusicTimeLength(music[musica]));
                }
            }

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadMusicStream(music[0]);
    UnloadMusicStream(music[1]);
    UnloadMusicStream(music[3]);
    UnloadTexture(capaAlbum[0]);
    UnloadTexture(capaAlbum[1]);
    UnloadTexture(capaAlbum[2]);
    // Unload music stream buffers from RAM

    CloseAudioDevice();         // Close audio device (music streaming is automatically stopped)

    CloseWindow();              // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
