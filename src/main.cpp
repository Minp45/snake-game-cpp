#include <iostream>
#include "raylib.h"
#include "Game.h"   // Include the Game class

using namespace std;

Color green = {173, 204, 96, 255};      // Background color
Color darkGreen = {43, 51, 24, 255};    // Snake and border color

int cellSize = 30;    // Size of one grid cell
int cellCount = 25;   // Number of cells along each axis
int offset = 75;      // Offset to center the grid

double lastUpdateTime = 0;   // Timer for movement and event updates

// Function to check if enough time has passed to trigger an event (like snake movement)
bool eventTriggered(double interval)
{
    double currentTime = GetTime();
    if (currentTime - lastUpdateTime >= interval) {
        lastUpdateTime = currentTime;
        return true;
    }
    return false;
}

int main() 
{
    cout << "Starting the game ..." << endl;

    // Initialize the window
    InitWindow(2 * offset + cellSize * cellCount, 2 * offset + cellSize * cellCount, "Retro Snake");
    SetTargetFPS(60);
    SetExitKey(0);  // Disable the exit key to handle game state changes manually

    // Create a game object
    Game game;

    // Main game loop
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(green);  // Set background color

        // Draw the game boundary
        DrawRectangleLinesEx(Rectangle{(float)offset - 5, (float)offset - 5, 
            (float)cellSize * cellCount + 10, (float)cellSize * cellCount + 10}, 5, darkGreen);

        // Title of the game at the top
        DrawText("Retro Snake", offset - 5, 20, 40, darkGreen);

        // Display the current score (top right corner)
        DrawText(TextFormat("Score: %i", game.score), offset + cellSize * cellCount - 110, 10, 25, darkGreen);
        
        // Display the best score (top right corner)
        DrawText(TextFormat("Best: %i", game.bestScore), offset + cellSize * cellCount - 110, 40, 25, darkGreen);

        // Update the background music if it is playing
        game.UpdateMusic();

        // Check if ESC key is pressed to toggle the pause state
        if (game.gameStarted && IsKeyPressed(KEY_ESCAPE)) {
            game.gamePaused = !game.gamePaused;  // Toggle pause state
            
            if (game.gamePaused) {
                PauseMusicStream(game.backGroundMusic);  // Pause the music
            } else {
                ResumeMusicStream(game.backGroundMusic);  // Resume the music
            }
        }

        // Draw the game elements (snake and food)
        game.Draw();

        // Handle initial screen before game starts
        if (!game.gameStarted) {
            // Draw a semi-transparent box for the start message
            DrawRectangle(offset + 20, offset + (cellSize * cellCount) / 2 - 40, 
                cellSize * cellCount - 40, 100, Fade(DARKGRAY, 0.7f));

            // Display "Press any arrow key to start" inside the box
            DrawText("Press any arrow key to start", offset + 50, offset + (cellSize * cellCount) / 2 - 10, 30, RAYWHITE);

            // Check if any arrow key is pressed to start the game
            if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_RIGHT)) {
                game.gamePaused = false;
                game.gameStarted = true;  // Set the game as started
                game.running = true;

                PlayMusicStream(game.backGroundMusic);  // Start playing the background music
            }
        }
        // If the game is paused, display the pause message
        else if (game.gamePaused) {
            // Draw a semi-transparent box for the pause message
            DrawRectangle(offset + 20, offset + (cellSize * cellCount) / 2 - 40, 
                cellSize * cellCount - 40, 100, Fade(DARKGRAY, 0.7f));

            // Display pause screen messages inside the box
            DrawText("Game Paused", offset + 50, offset + (cellSize * cellCount) / 2 - 20, 30, RAYWHITE);
            DrawText("Press any arrow key to resume", offset + 50, offset + (cellSize * cellCount) / 2 + 20, 20, RAYWHITE);

            // Resume the game if any arrow key is pressed
            if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_RIGHT)) {
                game.gamePaused = false;
                game.running = true;
                ResumeMusicStream(game.backGroundMusic);  // Resume the music when the game resumes
            }
        }
        // Otherwise, continue playing the game
        else {
            // If it's time to update (based on speed), update the game state
            if (eventTriggered(game.speed)) {
                game.Update();
            }

            // Handle player input for movement (snake direction)
            if (IsKeyPressed(KEY_UP) && game.snake.direction.y != 1) {
                game.snake.direction = {0, -1};
            }
            if (IsKeyPressed(KEY_DOWN) && game.snake.direction.y != -1) {
                game.snake.direction = {0, 1};
            }
            if (IsKeyPressed(KEY_LEFT) && game.snake.direction.x != 1) {
                game.snake.direction = {-1, 0};
            }
            if (IsKeyPressed(KEY_RIGHT) && game.snake.direction.x != -1) {
                game.snake.direction = {1, 0};
            }
        }

        EndDrawing();
    }

    CloseWindow();  // Close the window when the game exits
    return 0;
}
