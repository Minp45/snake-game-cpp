#ifndef SNAKE_H
#define SNAKE_H

#include <deque>
#include "raylib.h"  // Ensure you include the required Raylib header
#include <raymath.h> // For Vector2 functions

class Snake 
{
    public:
        std::deque<Vector2> body;
        Vector2 direction;
        bool addSegment;

        Snake();
        void Draw();
        void Update();
        void Reset();
};

#endif
