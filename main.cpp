#include <iostream>
#include "raylib.h"  // Use quotes for local libraries like Raylib, not angle brackets
#include <deque>
#include <raymath.h>

using namespace std;

Color green = {173, 204, 96, 255 };
Color darkGreen = {43, 51, 24, 255};

int cellSize = 30;
int cellCount = 25;
int offset = 75;

double lastUpdateTime = 0;

bool eventTriggered(double interval)
{
    double currentTime = GetTime();
    if(currentTime - lastUpdateTime >= interval)
    {
        lastUpdateTime =  currentTime;
        return true;
    }
    return false;
}

bool ElementInDeque(Vector2 element, deque<Vector2> deque)
{
    for(unsigned int i = 0; i < deque.size(); i++)
    {
        if(Vector2Equals(deque[i], element))
        {
            return true;
        }
    }
    return false;
}

class Snake 
{
    public:
        deque<Vector2> body = {Vector2{6, 9}, Vector2{5, 9}, Vector2{4, 9}};
        Vector2 direction = {1, 0};
        bool addSegment = false;

        void Draw()
        {
            for (unsigned int i = 0; i < body.size(); i++) {
                int x = (float)body[i].x;
                int y = (float)body[i].y;

                Rectangle segment = {(float)offset + x * cellSize, (float)offset +y * cellSize, 
                                     (float) cellSize, (float) cellSize};

                DrawRectangleRounded(segment, 0.5, 6, darkGreen);
            }
        }

        void Update()
        {
            body.push_front(Vector2Add(body[0], direction));
            if(addSegment == true)
            {
                addSegment = false;
            } else
            {
                body.pop_back();
            }
        }

        void Reset()
        {
            body = {Vector2{6,9}, Vector2{5,9}, Vector2{4,9}};
            direction = {1, 0};
        }

};

class Food 
{
    public:
        Vector2 position;
        Texture2D texture;

        Food(deque<Vector2> snakeBody)
        {
            Image image = LoadImage("graphics/food.png");
            texture = LoadTextureFromImage(image);
            UnloadImage(image);
            position = GenerateRandomPos(snakeBody);
        }

        ~Food(){
            UnloadTexture(texture);
        }

        void Draw()
        {
            Color white = {255, 255, 255, 255};
            DrawTexture(texture, offset + position.x * cellSize, offset + position.y * cellSize, white);
        }

        Vector2 GenerateRandomCell()
        {
            float x = GetRandomValue(0, cellCount - 1);
            float y = GetRandomValue(0, cellCount - 1);
            return Vector2{x, y};
        }

        Vector2 GenerateRandomPos(deque<Vector2> snakeBody)
        {
            Vector2 position = GenerateRandomCell();
            while(ElementInDeque(position, snakeBody))
            {
                position = GenerateRandomCell();
            }
            return position;
        }
};
 
class Game
{
    public:
        Snake snake = Snake();
        Food food = Food(snake.body);
        bool running = true;
        int score = 0;
        int bestScore = 0;
        Sound eatSound;
        Sound wallSound;
        Music backGroundMusic;
        bool gamePaused = true;  // Start the game in a paused state
        bool gameStarted = false;  // Track if the game has started
        double speed = 0.2;

        Game()
        {
            InitAudioDevice(); // Initialize audio device
            eatSound = LoadSound("sounds/eat.mp3");
            wallSound = LoadSound("sounds/wall.mp3");
            backGroundMusic = LoadMusicStream("sounds/happy_adveture.mp3");  // Load background music
        }
        
        ~Game()
        {
            UnloadSound(eatSound);
            UnloadSound(wallSound);
            UnloadMusicStream(backGroundMusic);  // Unload the music stream
            CloseAudioDevice();
        }

        void Draw()
        {
            food.Draw();
            snake.Draw();
        }
        
        void UpdateMusic()
        {
            UpdateMusicStream(backGroundMusic);  // Ensure the background music continues playing
        }

        void Update()
        {
            if (running)
            {
                snake.Update();
                CheckCollisionWithFood();
                CheckCollisionWithEdges();
                CheckCollisionWithTail();
            }
        }
        
        void CheckCollisionWithFood()
        {
            if(Vector2Equals(snake.body[0], food.position))
            {
                food.position = food.GenerateRandomPos(snake.body);
                snake.addSegment = true;
                score++;

                PlaySound(eatSound);

                if (speed > 0.05)
                {
                    speed *= 0.95;  // Increase the game speed by reducing the interval
                }
            }
        }

        void CheckCollisionWithEdges()
        {
            if (snake.body[0].x == cellCount || snake.body[0].x == -1 || snake.body[0].y == cellCount || snake.body[0].y == -1)
            {
                GameOver();
            }
        }

        void GameOver()
        {
            if (score > bestScore)  // Update the best score if the current score is higher
            {
                bestScore = score;
            }

            snake.Reset();
            food.position = food.GenerateRandomPos(snake.body);
            running = false;
            score = 0;
            speed = 0.2;
            PlaySound(wallSound);
            StopMusicStream(backGroundMusic);  // Stop the background music on game over

            gameStarted = false;
            gamePaused = true;
        }

        void CheckCollisionWithTail()
        {
            deque<Vector2> headlessBody = snake.body;
            headlessBody.pop_front();
            if (ElementInDeque(snake.body[0], headlessBody))
            {
                GameOver();
            }
        }
};

int main () 
{
    cout << "Starting the game ..." << endl;
    InitWindow(2*offset + cellSize*cellCount, 2*offset + cellSize*cellCount, "Retro Snake");
    SetTargetFPS(60);
    
    SetExitKey(0);

    Game game = Game();

    while (!WindowShouldClose())
    {
        BeginDrawing();

        ClearBackground(green);
        DrawRectangleLinesEx(Rectangle{(float)offset - 5, (float)offset - 5, (float)cellSize * cellCount + 10, (float)cellSize * cellCount + 10}, 5, darkGreen);
        
        // Title of the game
        DrawText("Retro Snake", offset - 5, 20, 40, darkGreen);

        // Display Score (Top right)
        DrawText(TextFormat("Score: %i", game.score), offset + cellSize * cellCount - 110, 10, 25, darkGreen);

        // Display Best Score (Top right)
        DrawText(TextFormat("Best: %i", game.bestScore), offset + cellSize * cellCount - 110, 40, 25, darkGreen);

        // Update the music stream to keep the background music playing
        game.UpdateMusic();

        // Check for ESC key to toggle pause state during gameplay
        if (game.gameStarted && IsKeyPressed(KEY_ESCAPE))
        {
            game.gamePaused = !game.gamePaused;  // Toggle gamePaused state
            
            if (game.gamePaused)
            {
                PauseMusicStream(game.backGroundMusic);  // Pause the music when the game is paused
            }
            else
            {
                ResumeMusicStream(game.backGroundMusic);  // Resume the music when the game is unpaused
            }
        }

        // Draw the snake and food, regardless of the game state
        game.Draw();

        // If game hasn't started yet (initial screen)
        if (!game.gameStarted)
        {
            // Draw a semi-transparent box for the start message
            DrawRectangle(offset + 20, offset + (cellSize * cellCount) / 2 - 40, cellSize * cellCount - 40, 100, Fade(DARKGRAY, 0.7f));

            // Display "Press any arrow key to start" inside the box
            DrawText("Press any arrow key to start", offset + 50, offset + (cellSize * cellCount) / 2 - 10, 30, RAYWHITE);

            // Check if any arrow key is pressed to start the game
            if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_RIGHT))
            {
                game.gamePaused = false;
                game.gameStarted = true;  // Set the game as started
                game.running = true;

                PlayMusicStream(game.backGroundMusic);  // Start playing the music when the game starts
            }
        }
        else if (game.gamePaused)
        {
            // Draw a semi-transparent box for the pause message
            DrawRectangle(offset + 20, offset + (cellSize * cellCount) / 2 - 40, cellSize * cellCount - 40, 100, Fade(DARKGRAY, 0.7f));
            
            // Display pause screen messages inside the box
            DrawText("Game Paused", offset + 50, offset + (cellSize * cellCount) / 2 - 20, 30, RAYWHITE);
            DrawText("Press any arrow key to resume", offset + 50, offset + (cellSize * cellCount) / 2 + 20, 20, RAYWHITE);

            // Resume the game if any arrow key is pressed
            if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_RIGHT))
            {
                game.gamePaused = false;
                game.running = true;
                ResumeMusicStream(game.backGroundMusic);  // Resume the music when the game resumes
            }
        }
        else
        {
            // If the game is not paused, update the game
            if (eventTriggered(game.speed))
            {
                game.Update();
            }

            // Handle player input for movement
            if (IsKeyPressed(KEY_UP) && game.snake.direction.y != 1)
            {
                game.snake.direction = {0, -1};
            }
            if (IsKeyPressed(KEY_DOWN) && game.snake.direction.y != -1)
            {
                game.snake.direction = {0, 1};
            }
            if (IsKeyPressed(KEY_LEFT) && game.snake.direction.x != 1)
            {
                game.snake.direction = {-1, 0};
            }
            if (IsKeyPressed(KEY_RIGHT) && game.snake.direction.x != -1)
            {
                game.snake.direction = {1, 0};
            }
        }

        EndDrawing();
    }

    CloseAudioDevice();  // Close the audio device at the end

    CloseWindow();
    return 0;
}
