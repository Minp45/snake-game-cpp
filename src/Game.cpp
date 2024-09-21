#include "Game.h"
#include "raylib.h"
#include "raymath.h" // For Vector2 functions
#include <deque>

extern int offset;
extern int cellSize;
extern int cellCount;
extern bool ElementInDeque(Vector2 element, std::deque<Vector2> deque);

Game::Game() : snake(), food(snake.body), running(true), score(0), bestScore(0), gamePaused(true), gameStarted(false), speed(0.2) 
{
    InitAudioDevice();
    eatSound = LoadSound("sounds/eat.mp3");
    wallSound = LoadSound("sounds/wall.mp3");
    backGroundMusic = LoadMusicStream("sounds/happy_adveture.mp3");
}

Game::~Game() 
{
    UnloadSound(eatSound);
    UnloadSound(wallSound);
    UnloadMusicStream(backGroundMusic);
    CloseAudioDevice();
}

void Game::Draw() 
{
    food.Draw();
    snake.Draw();
}

void Game::UpdateMusic() 
{
    UpdateMusicStream(backGroundMusic);
}

void Game::Update() 
{
    if (running) {
        snake.Update();
        CheckCollisionWithFood();
        CheckCollisionWithEdges();
        CheckCollisionWithTail();
    }
}

void Game::CheckCollisionWithFood() 
{
    if (Vector2Equals(snake.body[0], food.position)) {
        food.position = food.GenerateRandomPos(snake.body);
        snake.addSegment = true;
        score++;
        PlaySound(eatSound);

        if (speed > 0.05) {
            speed *= 0.95;
        }
    }
}

void Game::CheckCollisionWithEdges() 
{
    if (snake.body[0].x == cellCount || snake.body[0].x == -1 || snake.body[0].y == cellCount || snake.body[0].y == -1) {
        GameOver();
    }
}

void Game::GameOver() 
{
    if (score > bestScore) {
        bestScore = score;
    }

    snake.Reset();
    food.position = food.GenerateRandomPos(snake.body);
    running = false;
    score = 0;
    speed = 0.2;
    PlaySound(wallSound);
    StopMusicStream(backGroundMusic);

    gameStarted = false;
    gamePaused = true;
}

void Game::CheckCollisionWithTail() 
{
    std::deque<Vector2> headlessBody = snake.body;
    headlessBody.pop_front();
    if (ElementInDeque(snake.body[0], headlessBody)) {
        GameOver();
    }
}
