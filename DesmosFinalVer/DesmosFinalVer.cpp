#include "pch.h"
#include "IO.h"
#include "Equation.h"

//------------------------------------------------------------------------------------------
// Main entry point
//------------------------------------------------------------------------------------------



int main(void)
{

    // Initialization
    //--------------------------------------------------------------------------------------

    InitWindow(screenWidth, screenHeight, "Desmos");

    GameScreen currentScreen = GameScreen::Gameplay;
    Equation::Initialize(nodeCountBegin);
    SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor()));
    SetExitKey(KEY_NULL);
    //--------------------------------------------------------------------------------------
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        BeginDrawing();
        // Update
        //----------------------------------------------------------------------------------
        switch (currentScreen)
        {
        case GameScreen::Gameplay:
        {
            Equation::GraphingLogic();
            //vector<string> temp = Equation::CalculateString("3x");
        } break;

        default: break;
        }
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------


        ClearBackground(RAYWHITE);

        switch (currentScreen)
        {
        case GameScreen::Gameplay:
        {
            Equation::GraphDraw();
        } break;
        default: break;
        }

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------

    // TODO: Unload all loaded data (textures, fonts, audio) here!
    Equation::DeInitialize();
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
