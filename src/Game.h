#ifndef GAME_H
#define GAME_H

#include "Snake.h"
#include "Food.h"
#include "raylib.h"

class Game 
{
    public:
        Snake snake;
        Food food;
        bool running;
        int score;
        int bestScore;
        Sound eatSound;
        Sound wallSound;
        Music backGroundMusic;
        bool gamePaused;
        bool gameStarted;
        double speed;

        Game();
        ~Game();
        void Draw();
        void UpdateMusic();
        void Update();
        void CheckCollisionWithFood();
        void CheckCollisionWithEdges();
        void GameOver();
        void CheckCollisionWithTail();
};

#endif
