#include "Snake.h"
#include "raylib.h"  // Ensure this is included for the `DrawRectangleRounded` function

extern int cellSize;  // You need to declare extern for variables from other files
extern int offset;
extern Color darkGreen; // This color should also be available globally or passed as a parameter

Snake::Snake() : body({Vector2{6, 9}, Vector2{5, 9}, Vector2{4, 9}}), direction{1, 0}, addSegment(false)
{
}

void Snake::Draw() {
    for (unsigned int i = 0; i < body.size(); i++) {
        int x = static_cast<int>(body[i].x);
        int y = static_cast<int>(body[i].y);

        Rectangle segment = {(float)offset + x * cellSize, (float)offset + y * cellSize, (float)cellSize, (float)cellSize};
        DrawRectangleRounded(segment, 0.5, 6, darkGreen);
    }
}

void Snake::Update() {
    body.push_front(Vector2Add(body[0], direction));
    if (!addSegment) {
        body.pop_back();
    } else {
        addSegment = false;
    }
}

void Snake::Reset() {
    body = {Vector2{6, 9}, Vector2{5, 9}, Vector2{4, 9}};
    direction = {1, 0};
}
